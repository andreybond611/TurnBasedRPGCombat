// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DoesPathToLocationExist.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UBTTask_DoesPathToLocationExist : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Location;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Result;
};
