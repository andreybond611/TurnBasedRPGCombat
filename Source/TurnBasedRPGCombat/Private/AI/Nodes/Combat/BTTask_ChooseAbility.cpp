// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "AI/Nodes/Combat/BTTask_ChooseAbility.h"
#include "AIController.h"
#include "Abilities/AbilityComponent.h"
#include "Abilities/TargetTypes/AIAbilityTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TurnBased/TurnBasedBattle.h"

EBTNodeResult::Type UBTTask_ChooseAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const static FName CurrentBattleName = "CurrentBattle";

	Desirabilities.Empty();
	BehaviorTreeComponent = &OwnerComp;
	AActor* Owner = OwnerComp.GetOwner();
	AAIController* AIController = Cast<AAIController>(Owner);
	if (!AIController)
	{
		return EBTNodeResult::Aborted;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Aborted;
	}

	ATurnBasedBattle* CurrentBattle = Cast<ATurnBasedBattle>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(CurrentBattleName));
	if (!CurrentBattle)
	{
		return EBTNodeResult::Aborted;
	}

	if (auto AbilityComponent = ControlledPawn->FindComponentByClass<UAbilityComponent>())
	{
		TotalNumberOfAbilities = AbilityComponent->GetAllAbilities().Num();
		AbilityCount = 0;
		for (UAbility* Ability : AbilityComponent->GetAllAbilities())
		{
			UAIAbilityTarget* AITarget = Ability->GetAITarget();
			Ability->GetAITarget()->Init(ControlledPawn, CurrentBattle, OwnerComp.GetBlackboardComponent());
			AITarget->SetAbility(Ability);
			Ability->SetAIAsTarget();
			if (AITarget->GetIsAsync())
			{
				bSyncTasks = false;
				AITarget->OnDesirabilityFound.AddUObject(this, &UBTTask_ChooseAbility::AddDesirability);
				AITarget->CalculateDesirabilityAsync();
			}
			else
			{
				AbilityCount++;
				Desirabilities.Add(Ability, AITarget->CalculateDesirability());

			}
		}
	}

	if (bSyncTasks)
	{
		ChooseAbility();
		return EBTNodeResult::Succeeded;
	}

	bSyncTasks = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_ChooseAbility::ChooseAbility()
{
	float Highest = 0.f;
	UAbility* TargetAbility = nullptr;
	for (TTuple<UAbility*, float> Desirability : Desirabilities)
	{
		if (Desirability.Value > Highest)
		{
			Highest = Desirability.Value;
			TargetAbility = Desirability.Key;
		}
	}

	const static FName TargetAbilityName = "TargetAbility";

	(*BehaviorTreeComponent).GetBlackboardComponent()->SetValueAsObject(TargetAbilityName, TargetAbility);

	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}

void UBTTask_ChooseAbility::AddDesirability(UAbility* Ability, float Weight)
{
	AbilityCount++;
	Desirabilities.Add(Ability, Weight);

	if (AbilityCount == TotalNumberOfAbilities)
	{
		ChooseAbility();
	}
}
