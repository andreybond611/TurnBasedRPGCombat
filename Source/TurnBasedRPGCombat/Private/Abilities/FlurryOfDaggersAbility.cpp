// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/FlurryOfDaggersAbility.h"

#include "Abilities/Projectiles/ProjectileActor.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Surfaces/SurfaceActor.h"
#include "Utility/TurnBasedUtility.h"

void UFlurryOfDaggersAbility::StartAbility()
{
	Owner->PlayAnimMontage(AbilityMontage);

	ProjectilesHitNum = 0;
	OnAnimNotifyExecuted.AddDynamic(this, &UFlurryOfDaggersAbility::SpawnProjectiles);
}

void UFlurryOfDaggersAbility::EndAbility()
{
	Super::EndAbility();

	OnAnimNotifyExecuted.RemoveDynamic(this, &UFlurryOfDaggersAbility::SpawnProjectiles);
}

void UFlurryOfDaggersAbility::SpawnProjectiles()
{
	for (FVector Location : GetTarget().MultipleLocations)
	{
		const USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectileActor* Projectile = GetWorld()->SpawnActor<AProjectileActor>(*ProjectileClass, ProjectileStart->GetComponentLocation(),
																				ProjectileStart->GetComponentRotation(), Params);

		if (ensure(Projectile))
		{
			Projectile->SetProjectileOwner(Owner);
			Projectile->GetProjectileMovement()->Velocity = GetTossVelocity(Location);
			Projectile->OnProjectileHitDelegate.BindUObject(this, &UFlurryOfDaggersAbility::ProjectileHit);
		}
	}
}

FVector UFlurryOfDaggersAbility::GetTossVelocity(const FVector& TargetLocation) const
{
	const USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	FVector OutTossVelocity;

	UGameplayStatics::SuggestProjectileVelocity(this, OutTossVelocity, ProjectileStart->GetComponentLocation(), TargetLocation, LaunchSpeed,
												false, ProjectileRadius, 0.f, ESuggestProjVelocityTraceOption::OnlyTraceWhileAscending,
												FCollisionResponseParams::DefaultResponseParam, ActorsToIgnore, false);
	return OutTossVelocity;
}

void UFlurryOfDaggersAbility::ProjectileHit(AActor* HitActor, FVector Location)
{
	if (const auto Damageable = Cast<IDamageable>(HitActor))
	{
		const float DamageNumber = Owner->Stats()->Get(SN_Damage) * DamageMultiplier;
		const EHitDirection HitDirection = UTurnBasedUtility::FindHitDirection(Location, HitActor);

		FDamage Damage;
		Damage.DamageNumber = DamageNumber;
		Damage.HitDirection = HitDirection;
		Damage.DamageType = EDamageType::Poison;

		Damageable->GetDamaged(Damage);
	}

	FTransform SurfaceTransform;
	SurfaceTransform.SetLocation(Location);
	const auto PoisonSurface = GetWorld()->SpawnActorDeferred<ASurfaceActor>(*SurfaceActorClass, SurfaceTransform);
	PoisonSurface->SetSurfaceSize(SurfaceRadius);
	PoisonSurface->SetSurfaceType(SurfaceType);
	UGameplayStatics::FinishSpawningActor(PoisonSurface, SurfaceTransform);

	++ProjectilesHitNum;
	if (ProjectilesHitNum == GetTarget().MultipleLocations.Num())
	{
		EndAbility();
	}
}
