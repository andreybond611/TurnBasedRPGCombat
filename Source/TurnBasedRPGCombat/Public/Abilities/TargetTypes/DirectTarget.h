// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "DirectTarget.generated.h"

/**
 * Selects targets
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UDirectTarget : public UAbilityTargetState
{
	GENERATED_BODY()
public:
	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StopTargeting() override;

	void SetTargetMessage(const FText& Message) { TargetMessage = Message; }

private:
	UPROPERTY(EditAnywhere)
	float MinDistanceForRotation = 100.f;

	UPROPERTY(EditAnywhere)
	FText TargetMessage;
};
