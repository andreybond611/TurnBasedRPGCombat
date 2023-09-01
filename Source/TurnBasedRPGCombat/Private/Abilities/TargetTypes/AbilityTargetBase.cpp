// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/TargetTypes/AbilityTargetBase.h"
#include "GenericTeamAgentInterface.h"
#include "Abilities/Ability.h"

bool UAbilityTargetBase::IsDetected(AActor* Actor)
{
	if (const auto TeamAgentInterface = Cast<IGenericTeamAgentInterface>(Actor))
	{
		bool bIsEnemy = false;
		bool bIsNeutral = false;
		bool bIsFriendly = false;

		if (bDetectEnemy)
		{
			bIsEnemy = TeamAgentInterface->GetTeamAttitudeTowards(*AbilityOwner) == ETeamAttitude::Hostile;
		}
		if (bDetectNeutral)
		{
			bIsNeutral = TeamAgentInterface->GetTeamAttitudeTowards(*AbilityOwner) == ETeamAttitude::Neutral;
		}
		if (bDetectFriendly)
		{
			bIsFriendly = TeamAgentInterface->GetTeamAttitudeTowards(*AbilityOwner) == ETeamAttitude::Friendly;
		}

		return bIsEnemy || bIsFriendly || bIsNeutral || (Ability ? Ability->IsDetected(Actor) : true);
	}
	return false;
}
