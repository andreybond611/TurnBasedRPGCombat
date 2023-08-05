// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ReadiableAbility.h"
#include "ShinbiDashAbility.generated.h"

class UEffect;
/**
 *
 */
UCLASS(BlueprintType)
class TURNBASEDRPGCOMBAT_API UShinbiDashAbility : public UReadiableAbility
{
	GENERATED_BODY()
public:
	virtual void AddToCharacter(ARPGCharacter* Character) override;
	virtual void RemoveFromCharacter(ARPGCharacter* Character);
	virtual void StartAbility() override;

private:
	UPROPERTY(EditAnywhere, Category = "Ability Parameters")
	int32 TurnsOfEffect = 2;
	UPROPERTY(EditAnywhere, Category = "Ability Parameters")
	float DamageMultiplier = 1.8f;

	FVector PreviousLocation;

	UFUNCTION()
	void TeleportToTarget();
};
