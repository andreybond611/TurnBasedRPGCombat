// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "ArcTarget.generated.h"

class ASplineMeshActor;
class AProjectileSplineActor;
struct FPredictProjectilePathResult;
/**
 * Creates a mesh in form of an arc from projectile component to target location
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UArcTarget : public UAbilityTargetState
{
	GENERATED_BODY()
public:
	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;

	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StopTargeting() override;

	bool IsVelocityFound() const { return bSuggestedVelocityFound; }
	FVector GetTossVelocity() const { return TossVelocity; }
	FPredictProjectilePathResult GetPredictPathResult() const { return PredictPathResult; }
	void SetLaunchSpeed(float InLaunchSpeed) { LaunchSpeed = InLaunchSpeed; }
	float GetProjectileRadius() const { return ProjectileRadius; }

private:
	UPROPERTY(EditAnywhere)
	float ProjectileRadius = 1.f;
	UPROPERTY(EditAnywhere)
	float LaunchSpeed = 2000.f;
	UPROPERTY(EditAnywhere)
	float MeshScale = 0.1f;
	UPROPERTY(EditAnywhere)
	UStaticMesh* ProjectilePathMesh;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* ProjectilePathMaterial;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileSplineActor> ProjectileSplineActorClass;
	UPROPERTY(EditAnywhere)
	UStaticMesh* SplineEndMesh;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* SplineEndMeshMaterial;
	UPROPERTY(EditAnywhere)
	float SplineEndScale = 0.13f;

	FVector TossVelocity;

	FVector StartLocation;
	UPROPERTY()
	AProjectileSplineActor* SplineActor;

	FPredictProjectilePathResult PredictPathResult;

	bool bSuggestedVelocityFound;

	bool FindSuggestedVelocity(const FHitResult& HitResult, FVector& OutTossVelocity);
	bool PredictProjectilePath(FVector InTossVelocity, FPredictProjectilePathResult& OutResult);
	void AddMeshToLocation(FVector EndLocation);
	void CreateSplineMesh(USplineComponent* SplineComponent);
	void CreateArcSpline(FVector InTossVelocity, FPredictProjectilePathResult Result);
	void RemoveSpline();
};
