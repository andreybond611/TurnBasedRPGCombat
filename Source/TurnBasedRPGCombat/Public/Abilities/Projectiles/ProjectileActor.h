// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

class USphereComponent;
class ARPGCharacter;
class UProjectileMovementComponent;

DECLARE_DELEGATE_TwoParams(FOnProjectileHit, AActor*, FVector);

UCLASS()
class TURNBASEDRPGCOMBAT_API AProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	AProjectileActor();

	void SetProjectileOwner(AActor* InProjectileOwner);

	virtual void ProjectileHit();

	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovementComponent; }
	FOnProjectileHit OnProjectileHitDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticle;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
				   bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	AActor* ProjectileOwner;
};
