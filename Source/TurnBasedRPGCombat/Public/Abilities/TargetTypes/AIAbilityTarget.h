// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "Abilities/TargetTypes/AbilityTargetBase.h"
#include "Utility/TurnBasedTypes.h"
#include "AIAbilityTarget.generated.h"

class UBlackboardComponent;
class ATurnBasedBattle;
class UAbility;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFoundDesirability, UAbility*, float);

/**
 * Base class for targeting by AI
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class TURNBASEDRPGCOMBAT_API UAIAbilityTarget : public UAbilityTargetBase
{
	GENERATED_BODY()
public:
	virtual void Init(AActor* InAbilityOwner, ATurnBasedBattle* InContextBattle, UBlackboardComponent* InBlackboard);
	virtual float CalculateDesirability();
	virtual void CalculateDesirabilityAsync();

	FOnFoundDesirability OnDesirabilityFound;

	void SetMaxRange(float InMaxRange) { MaxRange = InMaxRange; }
	FTarget GetTarget() const { return Target; }
	bool GetIsAsync() const { return bAsync; }

protected:
	UPROPERTY(EditAnywhere)
	float MaxRange;

	UPROPERTY()
	ATurnBasedBattle* ContextBattle;

	FTarget Target;
	UPROPERTY()
	UNavigationSystemV1* NavSystem;
	UPROPERTY()
	UBlackboardComponent* Blackboard;

	bool bAsync = false;
};
