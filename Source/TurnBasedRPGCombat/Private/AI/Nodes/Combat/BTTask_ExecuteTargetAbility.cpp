// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/Nodes/Combat/BTTask_ExecuteTargetAbility.h"

#include "AIController.h"
#include "Abilities/AbilityComponent.h"
#include "BehaviorTree/BlackboardComponent.h"



EBTNodeResult::Type UBTTask_ExecuteTargetAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = &OwnerComp;
	Ability = Cast<UAbility>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetAbilityKey.SelectedKeyName));

	if (Ability)
	{
		const AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
		if (!AIController)
		{
			return EBTNodeResult::Aborted;
		}

		const APawn* Pawn = AIController->GetPawn();
		if (!Pawn)
		{
			return EBTNodeResult::Aborted;
		}

		if (const auto AbilityComponent = Pawn->FindComponentByClass<UAbilityComponent>())
		{
			AbilityComponent->ExecuteAbility(Ability);
			Ability->OnAbilityDoneExecuting.AddDynamic(this, &UBTTask_ExecuteTargetAbility::AbilityDone);
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_ExecuteTargetAbility::AbilityDone()
{
	Ability->OnAbilityDoneExecuting.RemoveDynamic(this, &UBTTask_ExecuteTargetAbility::AbilityDone);
	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}
