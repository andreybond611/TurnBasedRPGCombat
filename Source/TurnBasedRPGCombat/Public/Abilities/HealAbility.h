// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ReadiableAbility.h"
#include "HealAbility.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UHealAbility : public UReadiableAbility
{
	GENERATED_BODY()

public:
	UHealAbility();

	virtual void StartAbility() override;

	virtual void AddToCharacter(ARPGCharacter* Character) override;
	virtual void RemoveFromCharacter(ARPGCharacter* Character) override;

	virtual bool IsDetected(AActor* Target) override;

private:
	UFUNCTION()
	void ApplyEffect();
};
