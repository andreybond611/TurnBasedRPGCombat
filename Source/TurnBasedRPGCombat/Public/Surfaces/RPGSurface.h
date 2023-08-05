// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGSurface.generated.h"

class ARPGCharacter;
class USphereComponent;
class UNavStaticMeshComponent;
class UBaseSurface;
class UNavArea;

UCLASS()
class TURNBASEDRPGCOMBAT_API ARPGSurface : public AActor
{
	GENERATED_BODY()
	ARPGSurface();
public:
	void SetSurfaceSize(float InSize);
	void SetSurfaceType(TSubclassOf<UBaseSurface> InSurfaceType);
	TSubclassOf<UBaseSurface> GetSurfaceType() const { return SurfaceType; }
	UBaseSurface* GetSurface() const { return Surface; }
	TArray<ARPGSurface*> GetOverlappingSurfaces() const { return OverlappingSurfaces; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleInstanceOnly)
	TSubclassOf<UNavArea> NavAreaClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseSurface> SurfaceType;

	/** Size of collision mesh */
	UPROPERTY(EditAnywhere, Category = "Size")
	float ShapeSize = 150.f;
	UPROPERTY(EditAnywhere, Category = "Size")
	float SurfaceSize = 150.f;

	UPROPERTY(EditAnywhere)
	USceneComponent* Origin;
	UPROPERTY(EditAnywhere)
	UNavStaticMeshComponent* CollisionShape;
	UPROPERTY(EditAnywhere)
	UDecalComponent* Decal;

	UPROPERTY()
	UMaterialInstanceDynamic* DecalMaterialInstance;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* SurfaceMaterial;
	UPROPERTY(EditAnywhere)
	UTexture2D* TextureShape;

	UPROPERTY(VisibleInstanceOnly)
	TArray<ARPGSurface*> OverlappingSurfaces;

	UPROPERTY()
	TArray<AActor*> OverlappingActors;
	UPROPERTY()
	UBaseSurface* Surface;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CalculateSize();
};
