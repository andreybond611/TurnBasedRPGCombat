// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "SimpleAbility.h"
#include "Navigation/PathFollowingComponent.h"
#include "MeleeAttackAbility.generated.h"

class UMoveAbility;
/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UMeleeAttackAbility : public USimpleAbility
{
	GENERATED_BODY()
public:
	virtual void AddToCharacter(ARPGCharacter* Character) override;
	virtual void StartAbility() override;
	virtual void EndAbility() override;

private:
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> MeleeAttackMontages;
	UPROPERTY()
	UMoveAbility* MoveAbility;

	FDelegateHandle OnRequestFinishedHandle;

	UPROPERTY(EditAnywhere)
	EDamageType DamageType = EDamageType::Physical;

	void MeleeAttackLanded(AActor* TargetActor);
	void MeleeAttackDone();
	void MoveForMeleeAttack(AActor* TargetActor);
	void OnMoveToLocationFinished(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult);
	void OnMoveToActorFinished(FAIRequestID AIRequestID, const FPathFollowingResult& PathFollowingResult);

	UPROPERTY()
	AActor* MeleeTarget;
};

