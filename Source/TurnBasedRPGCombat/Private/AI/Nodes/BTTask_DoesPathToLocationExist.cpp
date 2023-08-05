// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/Nodes/BTTask_DoesPathToLocationExist.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_DoesPathToLocationExist::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetOwner<AAIController>();
	if (auto Pawn = AIController->GetPawn())
	{
		FVector PathStart = Pawn->GetActorLocation();
		FVector PathEnd = OwnerComp.GetBlackboardComponent()->GetValueAsVector(Location.SelectedKeyName);

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Pawn);
		FPathFindingQuery Query{Pawn, *NavSystem->GetAbstractNavData(), PathStart, PathEnd};

		bool bPathExists = NavSystem->TestPathSync(Query);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(Result.SelectedKeyName, bPathExists);
	}
	return EBTNodeResult::Succeeded;
}
