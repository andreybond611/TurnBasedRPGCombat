// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/Nodes/BTTask_DoesPathToLocationExist.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_DoesPathToLocationExist::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto AIController = OwnerComp.GetOwner<AAIController>();
	if (const auto Pawn = AIController->GetPawn())
	{
		const FVector PathStart = Pawn->GetActorLocation();
		const FVector PathEnd = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Location.SelectedKeyName);

		const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Pawn);
		const FPathFindingQuery Query{Pawn, *NavSystem->GetAbstractNavData(), PathStart, PathEnd};

		const bool bPathExists = NavSystem->TestPathSync(Query);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(Result.SelectedKeyName, bPathExists);
	}
	return EBTNodeResult::Succeeded;
}
