// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/FlurryOfDaggersAbility.h"

#include "Abilities/Projectiles/ProjectileActor.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/TurnBasedUtility.h"

void UFlurryOfDaggersAbility::PlayFlurryAnimation()
{
	const float AnimationLength = Owner->PlayAnimMontage(DaggerThrowAnimations[AnimationIndex]);
	AnimationIndex = AnimationIndex % DaggerThrowAnimations.Num();
	AnimationIndex++;
	DaggerThrowCount++;

	checkf(AnimationLength > 0.f, TEXT("Invalid animations in ability %s"), *GetName());
	FTimerHandle AnimationEndHandle;

	if (DaggerThrowCount >= NumberOfThrows)
	{
		GetWorld()->GetTimerManager().SetTimer(AnimationEndHandle, this, &UFlurryOfDaggersAbility::EndAbility, AnimationLength);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(AnimationEndHandle, this, &UFlurryOfDaggersAbility::ThrowDagger, AnimationLength * 0.5f);
	}
}

void UFlurryOfDaggersAbility::StartAbility()
{
	checkf(!DaggerThrowAnimations.IsEmpty(), TEXT("Ability %s doesn't contain anim montages"), *GetName());
	OnAnimNotifyExecuted.AddDynamic(this, &UFlurryOfDaggersAbility::SpawnProjectile);
	PlayFlurryAnimation();
}

void UFlurryOfDaggersAbility::EndAbility()
{
	Super::EndAbility();
	OnAnimNotifyExecuted.RemoveDynamic(this, &UFlurryOfDaggersAbility::SpawnProjectile);
	AnimationIndex = 0;
	DaggerThrowCount = 0;
}

void UFlurryOfDaggersAbility::ThrowDagger()
{
	PlayFlurryAnimation();
}

void UFlurryOfDaggersAbility::SpawnProjectile()
{
	const USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AProjectileActor* Projectile = GetWorld()->SpawnActor<AProjectileActor>(*ProjectileClass, ProjectileStart->GetComponentLocation(),
																			ProjectileStart->GetComponentRotation(), Params);

	if (ensure(Projectile))
	{
		Projectile->SetProjectileOwner(Owner);
		Projectile->GetProjectileMovement()->Velocity = GetTossVelocity();
		Projectile->OnProjectileHitDelegate.BindUObject(this, &UFlurryOfDaggersAbility::ProjectileHit);
	}
}

FVector UFlurryOfDaggersAbility::GetTossVelocity()
{
	const USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	FVector OutTossVelocity;

	UGameplayStatics::SuggestProjectileVelocity(this, OutTossVelocity, ProjectileStart->GetComponentLocation(), GetTarget().Location, LaunchSpeed,
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
}
