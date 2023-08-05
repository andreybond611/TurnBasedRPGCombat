// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilityTargetState.h"
#include "RangeTarget.generated.h"

class UMoveTarget;
class UArcTarget;
/**
 * Creates an arc when the cursor is on target
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API URangeTarget : public UAbilityTargetState
{
	GENERATED_BODY()
public:
	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;
	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StopTargeting() override;

	FVector GetTossVelocity() const { return TossVelocity; }

private:
	UPROPERTY(EditAnywhere, Instanced)
	UArcTarget* ProjectileArc;
	UPROPERTY(EditAnywhere, Instanced)
	UMoveTarget* MoveTarget;

	FVector TossVelocity;
};
