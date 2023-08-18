// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "AITypes.h"
#include "SimpleAbility.h"
#include "Navigation/PathFollowingComponent.h"
#include "ProjectileAbility.generated.h"

class AProjectileActor;
/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UProjectileAbility : public USimpleAbility
{
	GENERATED_BODY()
public:
	virtual void AddToCharacter(ARPGCharacter* Character) override;
	virtual void StartAbility() override;
	virtual void EndAbility() override;
private:
	FDelegateHandle OnRequestFinishedHandle;
	UPROPERTY(EditAnywhere)
	EDamageType DamageType = EDamageType::Physical;
	void OnMoveToLocationFinished(FAIRequestID FAIRequestID, const FPathFollowingResult& PathFollowingResult);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileActor> ProjectileClass;

	UPROPERTY(EditAnywhere)
	UAnimMontage* RangeAttackMontage;

	UPROPERTY(EditAnywhere)
	float LaunchSpeed = 2000.f;
	UPROPERTY(EditAnywhere)
	float ProjectileRadius = 1.f;

	UPROPERTY()
	UMoveAbility* MoveAbility;


	void ProjectileHit(AActor*HitActor, FVector ProjectileLocation);
	void ProjectileSpawn();
	UFUNCTION()
	void ShootProjectile();
};


