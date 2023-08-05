// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecuteTargetAbility.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UBTTask_ExecuteTargetAbility : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetAbilityKey;

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UPROPERTY()
	UAbility* Ability;
	UFUNCTION()
	void AbilityDone();
};
