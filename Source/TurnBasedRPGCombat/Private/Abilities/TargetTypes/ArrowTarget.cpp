// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/ArrowTarget.h"
#include "NavigationSystem.h"
#include "Characters/RPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealFramework/RPGPlayerController.h"
#include "NiagaraActor.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	FVector CharacterLocation = ControlledCharacter->GetLocation();
	FVector DiffVector = Location - CharacterLocation;
	FRotator LookAtRotation = DiffVector.Rotation();
	FRotator DecalRotation = {90.f, LookAtRotation.Yaw, 90.f};
	ArrowDecal = UGameplayStatics::SpawnDecalAtLocation(this, ArrowDecalInstance, ArrowDecalSize, Location, DecalRotation);

	// Put ArrowShaftDecal between ArrowDecal and Character
	FVector ShaftLocation = CharacterLocation + DiffVector * 0.5f;
	ArrowShaftDecalSize.Y = DiffVector.Size() * 0.5f;
	ArrowShaftDecal = UGameplayStatics::SpawnDecalAtLocation(this, ArrowShaftDecalInstance, ArrowShaftDecalSize, ShaftLocation, DecalRotation);
}

bool UArrowTarget::IsHitObstacle(const FVector& Location)
{
	static const FName IgnoreGroundProfileName = "IgnoreGround";
	static const FName IgnorePawnsAndGroundProfileName = "IgnorePawnsAndGround";

	TArray<FHitResult> OutHitResults;
	TArray<AActor*> ActorsToIgnore;
	bool bHitDestinationObstacle =
		UKismetSystemLibrary::SphereTraceMultiByProfile(PlayerController, Location, Location, CharacterRadius, IgnoreGroundProfileName, true,
														ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);

	bool bHitObstacleOnTheWay = UKismetSystemLibrary::SphereTraceMultiByProfile(PlayerController, ControlledCharacter->GetActorLocation(), Location,
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

	FVector StartLocation = ControlledCharacter->GetLocation();

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(PlayerController, StartLocation, Location, CharacterRadius, ObstacleObjectTypes,
																 false, ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHitResults)
		{
			if (IsDetected(Hit.GetActor()))
			{
				Target.MultipleActors.AddUnique(Hit.GetActor());
				ETeamAttitude::Type Attitude = ControlledCharacter->GetTeamAttitudeTowards(*Hit.GetActor());
				EOutlineColor Color = UTurnBasedUtility::AttitudeToOutlineColor(Attitude);
				EnableOutline(Hit.GetActor(), Color);
			}
		}
		return !Target.MultipleActors.IsEmpty();
	}
	return false;
}

void UArrowTarget::SetDecalColor(FLinearColor Color)
{
	static const FName ColorParameterName = "Color";
	ArrowDecalInstance->SetVectorParameterValue(ColorParameterName, Color);
	ArrowShaftDecalInstance->SetVectorParameterValue(ColorParameterName, Color);
}

void UArrowTarget::CheckMaxDistance(const FHitResult& HitResult)
{
	Target.Location = HitResult.Location;
	FVector CharacterLocation = ControlledCharacter->GetLocation();
	if (FVector::DistSquared(HitResult.Location, CharacterLocation) > MaxDistance * MaxDistance)
	{
		FVector DiffVector = HitResult.Location - CharacterLocation;
		FVector TargetDirection = DiffVector.GetSafeNormal();
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

	bool bWalkable = IsHitLocationWalkable(CursorHitResult);
	bool bHitObstacles = IsHitObstacle(Target.Location);
	bool bHitEnemy = IsHitTarget(Target.Location);

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

	UCapsuleComponent* CharacterCapsule = ControlledCharacter->GetCapsuleComponent();
	CharacterRadius = CharacterCapsule->GetScaledCapsuleRadius();

	ArrowShaftDecalInstance = UMaterialInstanceDynamic::Create(ArrowShaftDecalMaterial, this);
	ArrowDecalInstance = UMaterialInstanceDynamic::Create(ArrowDecalMaterial, this);
}

void UArrowTarget::StopTargeting()
{
	DestroyDecals();
	RemoveOutlineFromTargets();
}
