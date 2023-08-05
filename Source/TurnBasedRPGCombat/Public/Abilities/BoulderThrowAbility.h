// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/ReadiableAbility.h"
#include "BoulderThrowAbility.generated.h"

/**
 * Throws boulder projectile that hits medium-sized area, and creates a puddle of oil on impact
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UBoulderThrowAbility : public UReadiableAbility
{
	GENERATED_BODY()
public:
	virtual void ReadyAbility() override;
	virtual void StartAbility() override;
	virtual void CancelReadyAbility() override;
	virtual void EndAbility() override;

protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY(EditAnywhere, Category = "AbilityParameters")
	TSubclassOf<class ABoulderProjectile> BoulderProjectile;
	UPROPERTY(EditAnywhere, Category = "AbilityParameters")
	float ForwardProjectileStartOffset = 100.f;
	UPROPERTY(EditAnywhere, Category = "AbilityParameters")
	float LaunchSpeed = 1200.f;
	UPROPERTY(EditAnywhere, Category = "AbilityParameters")
	float InitialRelativeZ = -110.f;
	UPROPERTY(EditAnywhere, Category = "AbilityParameters")
	float ImpactSize = 100.f;
	UPROPERTY(EditAnywhere, Category = "AbilityParameters")
	TSubclassOf<class ARPGSurface> ImpactSurface;
	UPROPERTY(EditAnywhere, Category = "AbilityParameters")
	float DamageMultiplier = 1.5f;

	UPROPERTY()
	ABoulderProjectile* Boulder;

	float BoulderRadius = 30.f;

	void ProcessHits(const FVector& HitLocation, TArray<FHitResult> HitResults);
	void BoulderHit(AActor* Actor, FVector HitLocation);
	UFUNCTION()
	void ShootBoulder();
	UFUNCTION()
	void SpawnBoulder();

	FVector GetProjectileVelocity();
};
