// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/ProjectileAbility.h"

#include "Abilities/AbilityComponent.h"
#include "Abilities/Projectiles/ProjectileActor.h"
#include "Abilities/TargetTypes/RangeTarget.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealFramework/RPGPlayerController.h"
#include "Utility/TurnBasedUtility.h"

void UProjectileAbility::AddToCharacter(ARPGCharacter* Character)
{
	Super::AddToCharacter(Character);

	UAbilityComponent* AbilityComponent = Character->GetAbilityComponent();
	MoveAbility = AbilityComponent->GetMoveAbility();
}

void UProjectileAbility::StartAbility()
{
	if (auto* BattleParticipant = Cast<ITBBattleParticipant>(GetTarget().Actor))
	{
		if (ensure(RangeAttackMontage))
		{
			Owner->OnRotationEnded.AddDynamic(this, &UProjectileAbility::ShootProjectile);
			Owner->RotateToFaceActor(GetTarget().Actor);
		}
	}
	else
	{
		OnRequestFinishedHandle =
			Owner->FindPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UProjectileAbility::OnMoveToLocationFinished);
		Owner->MoveToLocation(GetTarget().Location);
	}
}

void UProjectileAbility::ProjectileHit(AActor* HitActor, FVector ProjectileLocation)
{
	if (auto Damageable = Cast<IDamageable>(HitActor))
	{
		EHitDirection HitDirection = UTurnBasedUtility::FindHitDirection(ProjectileLocation, HitActor);

		FDamage Damage;
		Damage.DamageNumber = Owner->Stats()->Get(SN_Damage);
		Damage.HitDirection = HitDirection;
		Damage.DamageType = DamageType;

		Damageable->GetDamaged(Damage);
	}
	EndAbility();
}

void UProjectileAbility::ProjectileSpawn()
{
	USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();

	FTransform Transform;
	Transform.SetLocation(ProjectileStart->GetComponentLocation());
	Transform.SetRotation(ProjectileStart->GetComponentRotation().Quaternion());
	AProjectileActor* Projectile =
		GetWorld()->SpawnActorDeferred<AProjectileActor>(*ProjectileClass, Transform, Owner, Owner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (ensure(Projectile))
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Owner);
		FVector OutTossVelocity;

		UGameplayStatics::SuggestProjectileVelocity(this, OutTossVelocity, ProjectileStart->GetComponentLocation(), GetTarget().Location, LaunchSpeed,
													false, ProjectileRadius, 0.f, ESuggestProjVelocityTraceOption::OnlyTraceWhileAscending,
													FCollisionResponseParams::DefaultResponseParam, ActorsToIgnore, false);

		Projectile->SetProjectileOwner(Owner);
		Projectile->OnProjectileHitDelegate.BindUObject(this, &UProjectileAbility::ProjectileHit);
		
		UGameplayStatics::FinishSpawningActor(Projectile, Transform);
		Projectile->GetProjectileMovement()->Velocity = OutTossVelocity;
	}
}

void UProjectileAbility::ShootProjectile()
{
	Owner->OnRotationEnded.RemoveDynamic(this, &UProjectileAbility::ShootProjectile);
	Owner->PlayAnimMontage(RangeAttackMontage);
	Owner->OnProjectileSpawn.BindUObject(this, &UProjectileAbility::ProjectileSpawn);
}

void UProjectileAbility::EndAbility()
{
	Super::EndAbility();
}

void UProjectileAbility::OnMoveToLocationFinished(FAIRequestID FAIRequestID, const FPathFollowingResult& PathFollowingResult)
{
	Owner->FindPathFollowingComponent()->OnRequestFinished.Remove(OnRequestFinishedHandle);
	EndAbility();
}
