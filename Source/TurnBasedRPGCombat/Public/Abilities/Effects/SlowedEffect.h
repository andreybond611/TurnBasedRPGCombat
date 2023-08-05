// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Effects/Effect.h"
#include "SlowedEffect.generated.h"

/**
 * Slows movement speed and movement animation
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API USlowedEffect : public UEffect
{
	GENERATED_BODY()
public:
	virtual void ApplyTo(AActor* InActor) override;
	virtual void Remove() override;
	virtual bool CanRemove() override;

private:
	UPROPERTY(EditAnywhere)
	float MovementMultiplier = -0.5f;
};
