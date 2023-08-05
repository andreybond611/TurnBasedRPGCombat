// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/SphereAroundTarget.h"

#include "Characters/RPGCharacter.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealFramework/RPGPlayerController.h"
#include "Utility/TurnBasedUtility.h"

void USphereAroundTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);

	CircleMaterialInstance = UMaterialInstanceDynamic::Create(CircleMaterial, this);
	SetCircleColor(OuterCircleDefaultColor, InnerCircleDefaultColor);
}

void USphereAroundTarget::HighlightActor(AActor* Actor)
{
	Target.MultipleActors.AddUnique(Actor);
	ETeamAttitude::Type Attitude = ControlledCharacter->GetTeamAttitudeTowards(*Actor);
	EOutlineColor OutlineColor = UTurnBasedUtility::AttitudeToOutlineColor(Attitude);

	EnableOutline(Actor, OutlineColor);
	SetCircleColor(OuterCircleEnemyColor, InnerCircleEnemyColor);
}

bool USphereAroundTarget::SphereTraceForPawnsAround(FVector OriginLocation)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<FHitResult> OutHitResults;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerController->GetControlledCharacter());

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(PlayerController, OriginLocation, OriginLocation, AbilityRadius, ObjectTypes, true,
																 ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);

	for (FHitResult HitResult : OutHitResults)
	{
		if (IsDetected(HitResult.GetActor()))
		{
			HighlightActor(HitResult.GetActor());
		}
	}

	return bHit;
}

void USphereAroundTarget::SetCircleColor(FLinearColor OuterColor, FLinearColor InnerColor)
{
	static const FName OuterColorParameter = "OuterColor";
	static const FName InnerColorParameter = "InnerColor";

	CircleMaterialInstance->SetVectorParameterValue(OuterColorParameter, OuterColor);
	CircleMaterialInstance->SetVectorParameterValue(InnerColorParameter, InnerColor);
}

void USphereAroundTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	FVector CharacterLocation = PlayerController->GetControlledCharacter()->GetLocation();

	if (CircleDecal)
	{
		CircleDecal->DestroyComponent();
		CircleDecal = nullptr;
	}
	Target.MultipleActors.Empty();
	DisableAllOutlines();

	CircleDecal = UGameplayStatics::SpawnDecalAtLocation(this, CircleMaterialInstance, DecalSize, CharacterLocation);

	SetCircleColor(OuterCircleDefaultColor, InnerCircleDefaultColor);

	SphereTraceForPawnsAround(CharacterLocation);
}

void USphereAroundTarget::StopTargeting()
{
	if (CircleDecal)
	{
		CircleDecal->DestroyComponent();
		CircleDecal = nullptr;
	}

	DisableAllOutlines();
}
