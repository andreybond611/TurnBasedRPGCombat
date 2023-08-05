// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileSplineActor.generated.h"

class USplineMeshComponent;
class USplineComponent;
/**
 * Creates meshes along spline
 */
UCLASS(BlueprintType)
class TURNBASEDRPGCOMBAT_API AProjectileSplineActor : public AActor
{
	GENERATED_BODY()

public:
	AProjectileSplineActor();

	USplineComponent* GetSplineComponent() const { return SplineComponent; }

	USplineMeshComponent* AddSplineMeshComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	USplineComponent* SplineComponent;
};
