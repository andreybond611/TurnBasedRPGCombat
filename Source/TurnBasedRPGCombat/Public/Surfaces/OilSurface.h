// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Surfaces/BaseSurface.h"
#include "OilSurface.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UOilSurface : public UBaseSurface
{
	GENERATED_BODY()
public:
	virtual void OnActorEntered(AActor* Actor) override;
	virtual void OnActorLeft(AActor* Actor) override;
	virtual void OnSurfaceIntersect(ASurfaceActor* OtherSurface) override;
};
