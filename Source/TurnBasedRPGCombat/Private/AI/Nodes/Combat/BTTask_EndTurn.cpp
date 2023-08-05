// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/Nodes/Combat/BTTask_EndTurn.h"

#include "AIController.h"
#include "ActorComponents/TurnBasedComponent.h"
#include "Characters/RPGCharacter.h"

EBTNodeResult::Type UBTTask_EndTurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetOwner<AAIController>();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	auto ControlledCharacter = AIController->GetPawn<ARPGCharacter>();
	if (!ControlledCharacter)
	{
		return EBTNodeResult::Failed;
	}

	ControlledCharacter->GetTurnBasedComponent()->EndTurn();
	return EBTNodeResult::Succeeded;
}
