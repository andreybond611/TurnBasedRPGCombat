// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "CharacterProgression/StatsComponent.h"
#include "CharacterStatsComponent.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UCharacterStatsComponent : public UStatsComponent
{
	GENERATED_BODY()
protected:
	virtual void InitializeStats() override;

private:
	void OnHeal(float HealedHealth);
};
