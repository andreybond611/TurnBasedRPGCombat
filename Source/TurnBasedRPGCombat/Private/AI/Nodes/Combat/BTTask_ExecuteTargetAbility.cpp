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
		AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
		APawn* Pawn = AIController->GetPawn();
		if (auto AbilityComponent = Pawn->FindComponentByClass<UAbilityComponent>())
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
