// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilityTargetState.h"
#include "ArcTarget.h"
#include "ProjectileRadiusTarget.generated.h"

class IOutlinable;
class AAOESphereActor;
class AStaticMeshActor;
class UArcTarget;
/**
 * Creates an arc and a sphere in target location
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UProjectileRadiusTarget : public UAbilityTargetState
{
	GENERATED_BODY()
	UProjectileRadiusTarget();

public:
	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;
	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StopTargeting() override;

	void SetSphereRadius(AAOESphereActor* AOESphere);

	FVector GetTossVelocity() const { return ArcTarget->GetTossVelocity(); }
	float GetProjectileRadius() const { return ArcTarget->GetProjectileRadius();  }
	void SetAOESphereRadius(const float InRadius) { AOESphereRadius = InRadius; }

private:
	UPROPERTY(EditAnywhere, Instanced)
	UArcTarget* ArcTarget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAOESphereActor> AOESphereClass;
	UPROPERTY(EditAnywhere)
	float AOESphereRadius = 150.f;

	UPROPERTY()
	AAOESphereActor* SpawnedAOESphere;
	FVector SphereScale;

	bool SphereTraceInLocation(FVector Location, TArray<FHitResult>& OutHitResults);
	void DetectTargets(TArray<FHitResult> OutHitResults);
	void DestroySphere();
};
