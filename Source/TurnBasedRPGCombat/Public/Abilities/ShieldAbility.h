// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ReadiableAbility.h"
#include "ShieldAbility.generated.h"

/**
 * Applies shield effect
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UShieldAbility : public UReadiableAbility
{
	GENERATED_BODY()
public:
	virtual void StartAbility() override;
	virtual void AddToCharacter(ARPGCharacter* Character) override;
	virtual void RemoveFromCharacter(ARPGCharacter* Character) override;

private:
	UPROPERTY(EditAnywhere, Category = "Ability Parameters")
	int32 Turns = 2;

	UFUNCTION()
	void ApplyEffect();
};
