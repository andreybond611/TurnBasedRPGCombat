// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChooseAbility.generated.h"

class UAbility;
/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UBTTask_ChooseAbility : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	int32 TotalNumberOfAbilities;
	int32 AbilityCount;
	bool bSyncTasks = true;

	UPROPERTY()
	TMap<UAbility*, float> Desirabilities;

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;

	void ChooseAbility();
	void AddDesirability(UAbility* Ability, float Weight);
};
