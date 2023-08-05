// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/ReadiableAbility.h"
#include "FlurryOfDaggersAbility.generated.h"

class AProjectileActor;
/**
 * Throws several projectiles
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UFlurryOfDaggersAbility : public UReadiableAbility
{
	GENERATED_BODY()
public:
	void PlayFlurryAnimation();
	virtual void StartAbility() override;
	virtual void EndAbility() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileActor> ProjectileClass;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> DaggerThrowAnimations;

	UPROPERTY(EditAnywhere)
	int32 NumberOfThrows = 3;
	UPROPERTY(EditAnywhere)
	float DamageMultiplier = 1.5f;
	UPROPERTY(EditAnywhere)
	float LaunchSpeed = 2000.f;
	UPROPERTY(EditAnywhere)
	float ProjectileRadius = 1.f;

	int32 DaggerThrowCount;
	int32 AnimationIndex;

	UFUNCTION()
	void ThrowDagger();
	UFUNCTION()
	void SpawnProjectile();
	UFUNCTION()
	void ProjectileHit(AActor* HitActor, FVector Location);

	FVector GetTossVelocity();
};


