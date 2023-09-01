// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Surfaces/BaseSurface.h"
#include "FireSurface.generated.h"

/**
 * Damages and puts on fire passing characters
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UFireSurface : public UBaseSurface
{
	GENERATED_BODY()
public:
	virtual void OnActorEntered(AActor* Actor) override;
	virtual void SurfaceTick(float DeltaTime) override;
	virtual void OnActorLeft(AActor* Actor) override;
	virtual void OnSurfaceIntersect(ASurfaceActor* OtherSurface) override;

private:
	UPROPERTY()
	TMap<AActor*, float> DistancePassedOnSurface;

	/** Distance character can pass without being damaged */
	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 150.f;
	UPROPERTY(EditAnywhere)
	float DamageOnMove = 12.f;
};
