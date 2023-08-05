// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/BoulderThrowAbility.h"

#include "Abilities/Effects/OnFireEffect.h"
#include "Abilities/Projectiles/BoulderProjectile.h"
#include "Abilities/TargetTypes/ProjectileRadiusTarget.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Surfaces/RPGSurface.h"
#include "Utility/TurnBasedUtility.h"

void UBoulderThrowAbility::ReadyAbility()
{
	Super::ReadyAbility();

	auto ProjectileRadiusTarget = Cast<UProjectileRadiusTarget>(GetTargetState());
	if (ensure(ProjectileRadiusTarget))
	{
		BoulderRadius = ProjectileRadiusTarget->GetProjectileRadius();
	}
	Owner->GetProjectileStartComponent()->AddRelativeLocation({ForwardProjectileStartOffset, 0.f, 0.f});
}

void UBoulderThrowAbility::StartAbility()
{
	OnAnimNotifyExecuted.AddDynamic(this, &UBoulderThrowAbility::SpawnBoulder);
	Owner->PlayAnimMontage(AbilityMontage);
}

void UBoulderThrowAbility::CancelReadyAbility()
{
	Super::CancelReadyAbility();

	Owner->GetProjectileStartComponent()->AddRelativeLocation({-ForwardProjectileStartOffset, 0.f, 0.f});
}

void UBoulderThrowAbility::EndAbility()
{
	Super::EndAbility();

	OnAnimNotifyExecuted.RemoveDynamic(this, &UBoulderThrowAbility::ShootBoulder);
	Owner->GetProjectileStartComponent()->AddRelativeLocation({-ForwardProjectileStartOffset, 0.f, 0.f});
}

#if WITH_EDITOR
void UBoulderThrowAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName ChangedPropertyName = PropertyChangedEvent.GetPropertyName();

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(UBoulderThrowAbility, ImpactSize))
	{
		auto ProjectileRadiusTarget = Cast<UProjectileRadiusTarget>(GetTargetState());
		if (ensure(ProjectileRadiusTarget))
		{
			ProjectileRadiusTarget->SetAOESphereRadius(ImpactSize);
		}
	}
}
#endif

FVector UBoulderThrowAbility::GetProjectileVelocity()
{
	USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	FVector OutTossVelocity;

	UGameplayStatics::SuggestProjectileVelocity(this, OutTossVelocity, ProjectileStart->GetComponentLocation(), GetTarget().Location, LaunchSpeed,
												false, BoulderRadius, 0.f, ESuggestProjVelocityTraceOption::DoNotTrace,
												FCollisionResponseParams::DefaultResponseParam, ActorsToIgnore, false);

	return OutTossVelocity;
}

void UBoulderThrowAbility::ProcessHits(const FVector& HitLocation, TArray<FHitResult> HitResults)
{
	TArray<AActor*> HitActors;
	for (FHitResult HitResult : HitResults)
	{
		if (auto Damageable = Cast<IDamageable>(HitResult.GetActor()))
		{
			HitActors.AddUnique(HitResult.GetActor());
		}
	}

	for (auto HitActor : HitActors)
	{
		float DamageNumber = Owner->Stats()->Get(SN_Damage) * DamageMultiplier;
		EHitDirection HitDirection = UTurnBasedUtility::FindHitDirection(HitLocation, HitActor);

		FDamage Damage;
		Damage.DamageNumber = DamageNumber;
		Damage.HitDirection = HitDirection;
		Damage.DamageType = EDamageType::Earth;

		auto DamageableActor = Cast<IDamageable>(HitActor);
		DamageableActor->GetDamaged(Damage);
	}
}

void UBoulderThrowAbility::BoulderHit(AActor* Actor, FVector HitLocation)
{
	auto SpawnedSurface = GetWorld()->SpawnActor<ARPGSurface>(*ImpactSurface, HitLocation, {});
	SpawnedSurface->SetSurfaceSize(ImpactSize);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<FHitResult> OutHitResults;

	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(this, HitLocation, HitLocation, ImpactSize, ObjectTypes, false,
																 ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);

	if (bHit)
	{
		ProcessHits(HitLocation, OutHitResults);
	}

	EndAbility();
}

void UBoulderThrowAbility::ShootBoulder()
{
	if (Boulder)
	{
		// Boulder collision with ground is ignored by default, so we should enable it when boulder is shot
		Boulder->GetCollisionComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3/*Ground*/, ECR_Block);

		FVector ProjectileVelocity = GetProjectileVelocity();
		Boulder->GetProjectileMovement()->Velocity = ProjectileVelocity;
		Boulder->SetProjectileOwner(Owner);
		Boulder->OnProjectileHitDelegate.BindUObject(this, &UBoulderThrowAbility::BoulderHit);
	}
}

void UBoulderThrowAbility::SpawnBoulder()
{
	OnAnimNotifyExecuted.RemoveDynamic(this, &UBoulderThrowAbility::SpawnBoulder);
	OnAnimNotifyExecuted.AddDynamic(this, &UBoulderThrowAbility::ShootBoulder);

	USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();
	FVector ProjectileSpawnLocation = ProjectileStart->GetComponentLocation();
	ProjectileSpawnLocation.Z += InitialRelativeZ;

	Boulder = GetWorld()->SpawnActor<ABoulderProjectile>(*BoulderProjectile, ProjectileSpawnLocation, FRotator{});
	Boulder->SetProjectileOwner(Owner);

	ensure(Boulder);
}
