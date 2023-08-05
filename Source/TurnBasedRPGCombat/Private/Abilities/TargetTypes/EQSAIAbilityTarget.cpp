// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/TargetTypes/EQSAIAbilityTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQuery.h"

void UEQSAIAbilityTarget::Init(AActor* InAbilityOwner, ATurnBasedBattle* InContextBattle, UBlackboardComponent* InBlackboard)
{
	Super::Init(InAbilityOwner, InContextBattle, InBlackboard);

	InBlackboard->SetValueAsObject(EnvQueryName, EnvQuery);
}

void UEQSAIAbilityTarget::SetEnvQuery(UEnvQuery* Query)
{
	EnvQuery = Query;
	Blackboard->SetValueAsObject(EnvQueryName, Query);
}
