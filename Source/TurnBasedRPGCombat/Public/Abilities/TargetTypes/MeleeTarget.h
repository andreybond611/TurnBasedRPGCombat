// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilityTargetState.h"
#include "NavigationPath.h"
#include "MeleeTarget.generated.h"


class UMoveTarget;
/**
 * Combines MoveTarget with ability to select enemy targets
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UMeleeTarget : public UAbilityTargetState
{
	GENERATED_BODY()
	UMeleeTarget();
public:
	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;
	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StopTargeting() override;
	virtual void TargetNotHit() override;



private:
	UPROPERTY(EditAnywhere, Instanced)
	UMoveTarget* MoveTarget;
};
