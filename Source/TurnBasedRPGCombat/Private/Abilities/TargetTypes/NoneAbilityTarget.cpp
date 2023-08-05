// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/TargetTypes/NoneAbilityTarget.h"

UNoneAbilityTarget* UNoneAbilityTarget::CreateNoneAbilityTarget()
{
	return NewObject<UNoneAbilityTarget>();
}
