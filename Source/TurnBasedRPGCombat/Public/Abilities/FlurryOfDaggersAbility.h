// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/ReadiableAbility.h"
#include "FlurryOfDaggersAbility.generated.h"

class UBaseSurface;
class AProjectileActor;
class ASurfaceActor;

/**
 * Throws several projectiles and creates poison surfaces under the hit locations
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UFlurryOfDaggersAbility : public UReadiableAbility
{
	GENERATED_BODY()
public:
	virtual void StartAbility() override;
	virtual void EndAbility() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileActor> ProjectileClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASurfaceActor> SurfaceActorClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseSurface> SurfaceType;
	UPROPERTY(EditAnywhere)
	float SurfaceRadius = 70.f;

	UPROPERTY(EditAnywhere)
	float DamageMultiplier = 1.5f;
	UPROPERTY(EditAnywhere)
	float LaunchSpeed = 2000.f;
	UPROPERTY(EditAnywhere)
	float ProjectileRadius = 1.f;

	int32 ProjectilesHitNum = 0;

	UFUNCTION()
	void SpawnProjectiles();
	UFUNCTION()
	void ProjectileHit(AActor* HitActor, FVector Location);

	FVector GetTossVelocity(const FVector& TargetLocation) const;
};


