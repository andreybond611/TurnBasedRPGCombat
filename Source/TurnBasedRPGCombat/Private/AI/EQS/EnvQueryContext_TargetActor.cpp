// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/EQS/EnvQueryContext_TargetActor.h"

#include "AI/TurnBasedAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/RPGCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const ARPGCharacter* QueryOwner = Cast<ARPGCharacter>(QueryInstance.Owner.Get());
	if (!QueryOwner)
	{
		return;
	}

	if (const auto AIController = QueryOwner->GetController<ATurnBasedAIController>())
	{
		if (const AActor* ContextCharacter = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor")))
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, ContextCharacter);
		}
	}
}
