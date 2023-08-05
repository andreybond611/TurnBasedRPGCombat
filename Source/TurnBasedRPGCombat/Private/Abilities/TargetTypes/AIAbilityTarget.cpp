// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/TargetTypes/AIAbilityTarget.h"

#include "NavigationSystem.h"

void UAIAbilityTarget::Init(AActor* InAbilityOwner, ATurnBasedBattle* InContextBattle, UBlackboardComponent* InBlackboard)
{
	ContextBattle = InContextBattle;
	AbilityOwner = InAbilityOwner;
	Blackboard = InBlackboard;

	NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
}

float UAIAbilityTarget::CalculateDesirability()
{
	return 0.f;
}

void UAIAbilityTarget::CalculateDesirabilityAsync()
{

}

