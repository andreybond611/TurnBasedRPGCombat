// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/ArrowTarget.h"
#include "Characters/RPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealFramework/RPGPlayerController.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/TurnBasedUtility.h"

UArrowTarget::UArrowTarget()
{
	bEnableRotating = true;
	CollisionChannel = ECC_GameTraceChannel2 /*GroundOnly*/;
}

void UArrowTarget::DestroyDecals()
{
	if (ArrowDecal)
	{
		ArrowDecal->DestroyComponent();
		ArrowDecal = nullptr;
	}
	if (ArrowShaftDecal)
	{
		ArrowShaftDecal->DestroyComponent();
		ArrowShaftDecal = nullptr;
	}
}

void UArrowTarget::SpawnDecals(const FVector& Location)
{
	const FVector CharacterLocation = ControlledCharacter->GetLocation();
	const FVector DiffVector = Location - CharacterLocation;
	const FRotator LookAtRotation = DiffVector.Rotation();
	const FRotator DecalRotation = {90.f, LookAtRotation.Yaw, 90.f};
	ArrowDecal = UGameplayStatics::SpawnDecalAtLocation(this, ArrowDecalInstance, ArrowDecalSize, Location, DecalRotation);

	// Put ArrowShaftDecal between ArrowDecal and Character
	const FVector ShaftLocation = CharacterLocation + DiffVector * 0.5f;
	ArrowShaftDecalSize.Y = DiffVector.Size() * 0.5f;
	ArrowShaftDecal = UGameplayStatics::SpawnDecalAtLocation(this, ArrowShaftDecalInstance, ArrowShaftDecalSize, ShaftLocation, DecalRotation);
}

bool UArrowTarget::IsHitObstacle(const FVector& Location)
{
	static const FName IgnoreGroundProfileName = "IgnoreGround";
	static const FName IgnorePawnsAndGroundProfileName = "IgnorePawnsAndGround";

	TArray<FHitResult> OutHitResults;
	const TArray<AActor*> ActorsToIgnore;
	const bool bHitDestinationObstacle =
		UKismetSystemLibrary::SphereTraceMultiByProfile(PlayerController, Location, Location, CharacterRadius, IgnoreGroundProfileName, true,
														ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);

	const bool bHitObstacleOnTheWay = UKismetSystemLibrary::SphereTraceMultiByProfile(PlayerController, ControlledCharacter->GetActorLocation(), Location,
	                                                                                  CharacterRadius, IgnorePawnsAndGroundProfileName, true,
	                                                                                  ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);

	return bHitDestinationObstacle || bHitObstacleOnTheWay;
}

void UArrowTarget::RemoveOutlineFromTargets()
{
	for (AActor* Actor : Target.MultipleActors)
	{
		DisableOutline(Actor);
	}
}

bool UArrowTarget::IsHitTarget(const FVector& Location)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleObjectTypes;
	ObstacleObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<FHitResult> OutHitResults;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(ControlledCharacter);

	const FVector StartLocation = ControlledCharacter->GetLocation();

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(PlayerController, StartLocation, Location, CharacterRadius, ObstacleObjectTypes,
	                                                                   false, ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHitResults)
		{
			if (IsDetected(Hit.GetActor()))
			{
				Target.MultipleActors.AddUnique(Hit.GetActor());
				const ETeamAttitude::Type Attitude = ControlledCharacter->GetTeamAttitudeTowards(*Hit.GetActor());
				const EOutlineColor Color = UTurnBasedUtility::AttitudeToOutlineColor(Attitude);
				EnableOutline(Hit.GetActor(), Color);
			}
		}
		return !Target.MultipleActors.IsEmpty();
	}
	return false;
}

void UArrowTarget::SetDecalColor(const FLinearColor Color)
{
	static const FName ColorParameterName = "Color";
	ArrowDecalInstance->SetVectorParameterValue(ColorParameterName, Color);
	ArrowShaftDecalInstance->SetVectorParameterValue(ColorParameterName, Color);
}

void UArrowTarget::CheckMaxDistance(const FHitResult& HitResult)
{
	Target.Location = HitResult.Location;
	const FVector CharacterLocation = ControlledCharacter->GetLocation();
	if (FVector::DistSquared(HitResult.Location, CharacterLocation) > MaxDistance * MaxDistance)
	{
		const FVector DiffVector = HitResult.Location - CharacterLocation;
		const FVector TargetDirection = DiffVector.GetSafeNormal();
		Target.Location = CharacterLocation + TargetDirection * MaxDistance;
		Target.Location.Z = HitResult.Location.Z;
	}
}

void UArrowTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	CheckMaxDistance(CursorHitResult);

	RemoveOutlineFromTargets();
	Target.MultipleActors.Empty();
	DestroyDecals();

	const bool bWalkable = IsHitLocationWalkable(CursorHitResult);
	const bool bHitObstacles = IsHitObstacle(Target.Location);
	const bool bHitEnemy = IsHitTarget(Target.Location);

	SpawnDecals(Target.Location);
	SetDecalColor(DefaultColor);

	if (!bWalkable || bHitObstacles)
	{
		SetDecalColor(BlockedPathColor);
		PlayerController->AddTargetInfoSection(PathIsBlockedText, 0, FColor::White);
		bAllowedExecute = false;
	}
	else
	{
		if (bHitEnemy)
		{
			SetDecalColor(HitEnemyColor);
		}
		bAllowedExecute = true;
	}
}

void UArrowTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);

	const UCapsuleComponent* CharacterCapsule = ControlledCharacter->GetCapsuleComponent();
	CharacterRadius = CharacterCapsule->GetScaledCapsuleRadius();

	ArrowShaftDecalInstance = UMaterialInstanceDynamic::Create(ArrowShaftDecalMaterial, this);
	ArrowDecalInstance = UMaterialInstanceDynamic::Create(ArrowDecalMaterial, this);
}

void UArrowTarget::StopTargeting()
{
	DestroyDecals();
	RemoveOutlineFromTargets();
}
