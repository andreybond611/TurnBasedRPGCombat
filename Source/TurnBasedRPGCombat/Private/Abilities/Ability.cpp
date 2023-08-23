// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/Ability.h"

#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "Abilities/TargetTypes/AIAbilityTarget.h"
#include "Characters/RPGCharacter.h"

void UAbility::StartAbility()
{
}

void UAbility::EndAbility()
{
	OnAbilityDoneExecuting.Broadcast();
}

void UAbility::AddToCharacter(ARPGCharacter* Character)
{
	//todo: make another function that called when init
	TargetState->SetAbility(this);
	Owner = Character;
}

void UAbility::RemoveFromCharacter(ARPGCharacter* Character)
{
	Owner = nullptr;
}

bool UAbility::IsDetected(AActor* Target)
{
	return false;
}

void UAbility::SetAIAsTarget()
{
	bAITarget = true;
}

FTarget UAbility::GetTarget() const
{
	return bAITarget ? GetAITarget()->GetTarget() : GetTargetState()->GetTarget();
}

