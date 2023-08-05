// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Projectiles/ProjectileActor.h"
#include "BoulderProjectile.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API ABoulderProjectile : public AProjectileActor
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void ProjectileHit() override;

	UFUNCTION(BlueprintNativeEvent)
	void EmergeFromGround();

	USphereComponent* GetCollisionComponent()const { return SphereCollision; }

	UPROPERTY(BlueprintReadWrite)
	float InitialRelativeZ = -110.f;
	UPROPERTY(BlueprintReadWrite)
	float HighestRelativeZ = 210.f;
};
