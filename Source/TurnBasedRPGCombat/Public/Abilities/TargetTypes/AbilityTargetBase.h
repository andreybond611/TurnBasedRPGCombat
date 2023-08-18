// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilityTargetBase.generated.h"

class UAbility;
/**
 * Base class for targeting
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UAbilityTargetBase : public UObject
{
	GENERATED_BODY()
public:
	void SetAbility(UAbility* InAbility) { Ability = InAbility; }

	bool IsDetected(AActor* Actor);

protected:
	UPROPERTY()
	UAbility* Ability;
	UPROPERTY()
	AActor* AbilityOwner;

	UPROPERTY(EditAnywhere)
	bool bDetectFriendly;
	UPROPERTY(EditAnywhere)
	bool bDetectNeutral;
	UPROPERTY(EditAnywhere)
	bool bDetectEnemy = true;
};
