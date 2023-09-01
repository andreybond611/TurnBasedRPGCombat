// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BaseSurface.generated.h"

class ASurfaceActor;
class UEffect;
class UNavArea;

/**
 * Base class for surface implementation
 */
UCLASS(BlueprintType, Blueprintable)
class TURNBASEDRPGCOMBAT_API UBaseSurface : public UObject
{
	GENERATED_BODY()
public:
	UBaseSurface();

	virtual void Init(ASurfaceActor* InSurfaceActor);
	virtual void OnActorEntered(AActor* Actor);
	virtual void SurfaceTick(float DeltaTime);
	virtual void OnActorLeft(AActor* Actor);
	virtual void OnSurfaceIntersect(ASurfaceActor* OtherSurface);

	TSubclassOf<UNavArea> GetNavArea() const { return NavAreaClass; }

	UMaterialInterface* GetMaterial() const { return Material; }

	void ChangeSurfaceType(TSubclassOf<UBaseSurface> InSurfaceType);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UNavArea> NavAreaClass;
	UPROPERTY()
	ASurfaceActor* SurfaceActor;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SurfaceTag;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEffect> SurfaceEffectClass;

};
