// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/Nodes/BTTask_FindLocationCloseToEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TurnBased/TurnBasedBattle.h"
#include "TurnBased/TurnBasedBattleParticipant.h"

EBTNodeResult::Type UBTTask_FindLocationCloseToEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const static FName CurrentBattleName = "CurrentBattle";

	auto AIController = OwnerComp.GetOwner<AAIController>();
	if (auto Pawn = AIController->GetPawn())
	{
		ATurnBasedBattle* Battle = Cast<ATurnBasedBattle>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(CurrentBattleName));
		if (!Battle)
		{
			return EBTNodeResult::Aborted;
		}

		TArray<TScriptInterface<ITBBattleParticipant>> Enemies = Battle->GetParticipantsWithAttitudeTo(Pawn, ETeamAttitude::Hostile);

		float MinDistance = TNumericLimits<float>::Max();
		AActor* ClosestActor = nullptr;
		for (auto BattleParticipant : Enemies)
		{
			float Distance = FVector::DistSquared(Pawn->GetActorLocation(), BattleParticipant->GetLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestActor = BattleParticipant->GetActor();
			}
		}

		if (ClosestActor)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ResultLocation.SelectedKeyName, ClosestActor->GetActorLocation());
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(ResultActor.SelectedKeyName, ClosestActor);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Aborted;
}
