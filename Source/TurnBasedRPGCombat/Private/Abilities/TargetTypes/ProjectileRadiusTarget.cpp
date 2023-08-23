// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/ProjectileRadiusTarget.h"
#include "Abilities/TargetTypes/ArcTarget.h"
#include "Abilities/TargetTypes/Helpers/AOESphereActor.h"
#include "Components/SphereComponent.h"
#include "Characters/RPGCharacter.h"

UProjectileRadiusTarget::UProjectileRadiusTarget()
{
	CollisionChannel = ECollisionChannel::ECC_GameTraceChannel2 /*GroundOnly*/;
}

void UProjectileRadiusTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);

	ArcTarget->StartTargeting(InPlayerController);
}

void UProjectileRadiusTarget::SetSphereRadius()
{
	const float SphereRadius = SpawnedAOESphere->GetSphereCollision()->GetUnscaledSphereRadius();
	const float Scale = AOESphereRadius / SphereRadius;
	SphereScale = SpawnedAOESphere->GetActorScale3D();
	SphereScale.X = Scale;
	SphereScale.Y = Scale;
	SphereScale.Z = Scale > 2.f ? Scale / 2.f : SphereScale.Z;
	SpawnedAOESphere->SetActorScale3D(SphereScale);
}

bool UProjectileRadiusTarget::SphereTraceInLocation(FVector Location, TArray<FHitResult>& OutHitResults)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(ControlledCharacter);
	return UKismetSystemLibrary::SphereTraceMultiForObjects(this, Location, Location, AOESphereRadius, ObjectTypes, false, ActorsToIgnore,
															EDrawDebugTrace::None, OutHitResults, true);
}

void UProjectileRadiusTarget::DetectTargets(TArray<FHitResult> OutHitResults)
{
	for (FHitResult HitResult : OutHitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (auto BattleParticipant = Cast<ITBBattleParticipant>(HitActor))
		{
			if (IsDetected(HitActor))
			{
				EnableOutline(HitActor, EOutlineColor::Red);
				Target.MultipleActors.AddUnique(HitActor);
			}
		}
	}
}

void UProjectileRadiusTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	DisableAllOutlines();
	Target.MultipleActors.Empty();

	ArcTarget->TickTargetAbility(CursorHitResult);
	FVector AOESphereLocation = ArcTarget->GetPredictPathResult().HitResult.Location;

	Target.Location = CursorHitResult.Location;
	bAllowedExecute = false;

	if (ArcTarget->IsVelocityFound())
	{
		bAllowedExecute = true;

		// AOE sphere should spawn only on the ground, so if arc hit something higher on the way, then we lower sphere location to the ground
		if (!FMath::IsNearlyEqual(AOESphereLocation.Z, CursorHitResult.Location.Z))
		{
			AOESphereLocation.Z = CursorHitResult.Location.Z;
			ArcTarget->StopTargeting();
			FHitResult NewHitResult = CursorHitResult;
			NewHitResult.Location = AOESphereLocation;
			ArcTarget->TickTargetAbility(NewHitResult);
			Target.Location = NewHitResult.Location;
		}

		if (!SpawnedAOESphere)
		{
			SpawnedAOESphere = GetWorld()->SpawnActor<AAOESphereActor>(*AOESphereClass, AOESphereLocation, FRotator{});
		}

		if (ensure(SpawnedAOESphere))
		{
			SpawnedAOESphere->SetActorLocation(AOESphereLocation);
			SetSphereRadius();

			TArray<FHitResult> OutHitResults;
			const bool bHit = SphereTraceInLocation(AOESphereLocation, OutHitResults);
			if (bHit)
			{
				DetectTargets(OutHitResults);
			}
		}
	}
	else
	{
		if (SpawnedAOESphere)
		{
			SpawnedAOESphere->Destroy();
			SpawnedAOESphere = nullptr;
		}
	}
}

void UProjectileRadiusTarget::DestroySphere()
{
	if (SpawnedAOESphere)
	{
		SpawnedAOESphere->Destroy();
		SpawnedAOESphere = nullptr;
	}
}

void UProjectileRadiusTarget::StopTargeting()
{
	Super::StopTargeting();

	ArcTarget->StopTargeting();
	DestroySphere();
	DisableAllOutlines();
}
