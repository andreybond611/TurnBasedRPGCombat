// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/Nodes/BTTask_SetLocationWhenNoPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_SetLocationWhenNoPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetOwner<AAIController>();
	if (auto Pawn = AIController->GetPawn())
	{
		FVector PathStart = Pawn->GetActorLocation();
		FVector PathEnd = OwnerComp.GetBlackboardComponent()->GetValueAsVector(PathDestination.SelectedKeyName);
		FVector HitLocation;
		UNavigationSystemV1::NavigationRaycast(Pawn, PathStart, PathEnd, HitLocation);

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Result.SelectedKeyName, HitLocation);
	}

	return EBTNodeResult::Succeeded;
}
