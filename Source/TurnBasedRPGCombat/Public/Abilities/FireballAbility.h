// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ReadiableAbility.h"
#include "FireballAbility.generated.h"

class ASurfaceActor;
class UBaseSurface;
class AProjectileActor;
/**
 * Shoots fireball, creates a fire surface on impact
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UFireballAbility : public UReadiableAbility
{
	GENERATED_BODY()
public:
	virtual void StartAbility() override;
	FVector GetProjectileVelocity();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileActor> ProjectileClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASurfaceActor> FireSurfaceActorClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseSurface> SurfaceType;
	UPROPERTY(EditAnywhere)
	float Radius = 180.f;
	UPROPERTY(EditAnywhere)
	float DamageMultiplier = 5.f;
	UPROPERTY(EditAnywhere)
	float LaunchSpeed = 5000.f;
	UPROPERTY(EditAnywhere)
	float ProjectileRadius = 1.f;

	UPROPERTY()
	AProjectileActor* ShotProjectile;
	UPROPERTY()
	TArray<AActor*> HitActors;


	UFUNCTION()
	void ShootFireball();

	void FireballHit(AActor* HitActor, FVector Location);

	void ProcessHits(FVector Location, const TArray<FHitResult>& OutHitResults);
};
