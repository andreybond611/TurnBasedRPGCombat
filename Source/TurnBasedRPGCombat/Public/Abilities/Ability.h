// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Utility/TurnBasedTypes.h"
#include "Ability.generated.h"

class UAIAbilityTarget;
class UAbilityTargetState;
class UEffect;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityDoneExecuting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimNotifyExecuted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationEnd);

class ARPGCharacter;
/**
 * Base class for abilities
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class TURNBASEDRPGCOMBAT_API UAbility : public UObject
{
	GENERATED_BODY()
public:
	virtual void StartAbility();
	virtual void EndAbility();

	virtual void AddToCharacter(ARPGCharacter* Character);
	virtual void RemoveFromCharacter(ARPGCharacter* Character);

	/* Overrides the default detection function IsDetected in AbilityTargetState */
	virtual bool IsDetected(AActor* Target);

	void SetAIAsTarget();

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDoneExecuting OnAbilityDoneExecuting;
	UPROPERTY(BlueprintAssignable)
	FOnAnimNotifyExecuted OnAnimNotifyExecuted;
	UPROPERTY(BlueprintAssignable)
	FOnAnimationEnd OnAnimationEnd;

	float GetRange() const { return Range; }
	int32 GetAPCost() const { return APCost; }
	float GetCooldown() const { return Cooldown; }
	float GetRemainingCooldown() const { return RemainingCooldown; }
	UAbilityTargetState* GetTargetState() const { return TargetState; }
	UAIAbilityTarget* GetAITarget() const { return AITargetState; }
	FTarget GetTarget() const;

protected:
	UPROPERTY(EditAnywhere, Category = "Ability Parameters")
	float Range;
	UPROPERTY(EditAnywhere, Category = "Ability Parameters")
	int32 APCost;
	UPROPERTY(EditAnywhere, Category = "Ability Parameters")
	FText Name;
	UPROPERTY(EditAnywhere, meta = (MultiLine = "true"), Category = "Ability Parameters")
	FText Description;
	UPROPERTY(EditAnywhere, Category = "Ability Parameters")
	float Cooldown;
	UPROPERTY(EditInstanceOnly)
	float RemainingCooldown;

	UPROPERTY(EditAnywhere, Instanced)
	UAbilityTargetState* TargetState;

	UPROPERTY(EditAnywhere, Instanced)
	UAIAbilityTarget* AITargetState;

	UPROPERTY()
	ARPGCharacter* Owner;
	bool bAITarget = false;
};


