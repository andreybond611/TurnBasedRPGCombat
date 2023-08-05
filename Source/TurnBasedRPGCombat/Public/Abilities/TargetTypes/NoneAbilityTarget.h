// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "NoneAbilityTarget.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UNoneAbilityTarget : public UAbilityTargetState
{
	GENERATED_BODY()
public:
	static UNoneAbilityTarget* CreateNoneAbilityTarget();
};
