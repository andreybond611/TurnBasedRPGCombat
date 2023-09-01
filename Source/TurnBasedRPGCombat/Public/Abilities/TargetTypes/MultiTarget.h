// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "MultiTarget.generated.h"

/**
 * Allows to choose multiple targets with an each click
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UMultiTarget : public UAbilityTargetState
{
	GENERATED_BODY()
public:
	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;
	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StopTargeting() override;

	virtual void ConfirmTarget();
	virtual void CancelConfirmedTarget();
	virtual bool CanConfirmTarget();

protected:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"))
	int32 MaxTargetNum = 2;

	int32 TargetNum = 0;

	UPROPERTY(EditAnywhere, Instanced)
	UAbilityTargetState* SingleAbilityTarget;
};
