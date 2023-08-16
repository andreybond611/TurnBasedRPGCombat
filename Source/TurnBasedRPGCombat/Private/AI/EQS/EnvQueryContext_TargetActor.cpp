// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/EQS/EnvQueryContext_TargetActor.h"

#include "EngineUtils.h"
#include "AI/TurnBasedAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/RPGCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

void UEnvQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	ARPGCharacter* QueryOwner = Cast<ARPGCharacter>(QueryInstance.Owner.Get());
	if (!QueryOwner)
	{
		return;
	}

	if (auto AIController = QueryOwner->GetController<ATurnBasedAIController>())
	{
		if (AActor* ContextCharacter = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor")))
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, ContextCharacter);
		}
	}
}
