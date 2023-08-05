// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ReadiableAbility.h"
#include "WhirlwindAbility.generated.h"

/**
 *	Hits targets around in radius
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UWhirlwindAbility : public UReadiableAbility
{
	GENERATED_BODY()
public:
	virtual void StartAbility() override;
	virtual void AddToCharacter(ARPGCharacter* Character) override;
	virtual void RemoveFromCharacter(ARPGCharacter* Character) override;

private:
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* WhirlwindEffect;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* Impact;
	UPROPERTY(EditAnywhere, Category = "VFX")
	float WhirlwindEffectLifetimeSeconds = 1.1f;
	UPROPERTY(EditAnywhere, Category = "Ability Parameters")
	float DamageMultiplier = 1.3f;

	UFUNCTION()
	void HitEveryoneAround();
};
