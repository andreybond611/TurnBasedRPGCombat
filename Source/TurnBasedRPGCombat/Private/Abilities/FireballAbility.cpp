// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/FireballAbility.h"

#include "NiagaraComponent.h"
#include "Abilities/Effects/EffectComponent.h"
#include "Abilities/Effects/OnFireEffect.h"
#include "Abilities/Projectiles/ProjectileActor.h"
#include "Abilities/TargetTypes/ProjectileRadiusTarget.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Surfaces/RPGSurface.h"
#include "Utility/TurnBasedUtility.h"

void UFireballAbility::StartAbility()
{
	OnAnimNotifyExecuted.AddUniqueDynamic(this, &UFireballAbility::ShootFireball);
	Owner->PlayAnimMontage(AbilityMontage);
}

FVector UFireballAbility::GetProjectileVelocity()
{
	USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	FVector OutTossVelocity;

	UGameplayStatics::SuggestProjectileVelocity(this, OutTossVelocity, ProjectileStart->GetComponentLocation(), GetTarget().Location, LaunchSpeed,
												false, ProjectileRadius, 0.f, ESuggestProjVelocityTraceOption::OnlyTraceWhileAscending,
												FCollisionResponseParams::DefaultResponseParam, ActorsToIgnore, false);
	return OutTossVelocity;
}

void UFireballAbility::ShootFireball()
{
	USceneComponent* ProjectileStart = Owner->GetProjectileStartComponent();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ShotProjectile =
		GetWorld()->SpawnActor<AProjectileActor>(*ProjectileClass, ProjectileStart->GetComponentLocation(), ProjectileStart->GetComponentRotation(), Params);

	if (ensure(ShotProjectile))
	{
		FVector ProjectileVelocity = GetProjectileVelocity();
		ShotProjectile->GetProjectileMovement()->Velocity = ProjectileVelocity;
		ShotProjectile->SetProjectileOwner(Owner);
		ShotProjectile->OnProjectileHitDelegate.BindUObject(this, &UFireballAbility::FireballHit);
	}
}

void UFireballAbility::ProcessHits(FVector Location, const TArray<FHitResult>& OutHitResults)
{
	for (FHitResult HitResult : OutHitResults)
	{
		if (auto Damageable = Cast<IDamageable>(HitResult.GetActor()))
		{
			HitActors.AddUnique(HitResult.GetActor());
		}
	}

	for (auto HitActor : HitActors)
	{
		float DamageNumber = Owner->Stats()->Get(SN_Damage) * DamageMultiplier;
		EHitDirection HitDirection = UTurnBasedUtility::FindHitDirection(Location, HitActor);

		FDamage Damage;
		Damage.DamageNumber = DamageNumber;
		Damage.HitDirection = HitDirection;
		Damage.DamageType = EDamageType::Fire;

		auto DamageableActor = Cast<IDamageable>(HitActor);
		DamageableActor->GetDamaged(Damage);

		UOnFireEffect* Effect = NewObject<UOnFireEffect>(this, *AppliedEffectClass);
		auto EffectComponent = HitActor->FindComponentByClass<UEffectComponent>();
		EffectComponent->AddEffect(Effect);
	}
}

void UFireballAbility::FireballHit(AActor* HitActor, FVector Location)
{
	ShotProjectile->OnProjectileHitDelegate.Unbind();

	FTransform SurfaceTransform;
	SurfaceTransform.SetLocation(Location);
	auto FireSurface = GetWorld()->SpawnActorDeferred<ARPGSurface>(*FireSurfaceActorClass, SurfaceTransform);
	FireSurface->SetSurfaceSize(Radius);
	FireSurface->SetSurfaceType(SurfaceType);
	UGameplayStatics::FinishSpawningActor(FireSurface, SurfaceTransform);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<FHitResult> OutHitResults;

	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(this, Location, Location, Radius, ObjectTypes, false,
																 ActorsToIgnore, EDrawDebugTrace::None, OutHitResults, true);

	if (bHit)
	{
		ProcessHits(Location, OutHitResults);
	}

	EndAbility();
}