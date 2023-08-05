// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/ProjectileRadiusTarget.h"
#include "GameFramework/Actor.h"
#include "AOESphereActor.generated.h"

class USphereComponent;
/**
 * Sphere for targeting AOE abilities
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API AAOESphereActor : public AActor
{
	GENERATED_BODY()

public:
	AAOESphereActor();

	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
	USphereComponent* GetSphereCollision() const { return SphereCollision; }

	void SetOverlapEnemy(bool bOverlap);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* SphereMaterial;
	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor = FLinearColor{ 1.f, 1.f, 1.f, 0.3f };
	UPROPERTY(EditAnywhere)
	FLinearColor DetectedColor = FLinearColor{ 0.f, 1.f, 0.f, 0.3f };


	UPROPERTY()
	UProjectileRadiusTarget* ProjectileRadiusTarget;
	UPROPERTY()
	UMaterialInstanceDynamic* SphereMaterialInstance;
	UPROPERTY()
	TArray<ARPGCharacter*> DetectedCharacters;

};
