// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetLocationWhenNoPath.generated.h"

/**
 * Gives closest location to PathDestination near obstacles. Use only when no path to PathDestination exists
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UBTTask_SetLocationWhenNoPath : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PathDestination;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Result;
};
