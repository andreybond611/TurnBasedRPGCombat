// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityInputConfigData.generated.h"

class UInputAction;
/**
 * Stores ability inputs
 */
UCLASS(Blueprintable)
class TURNBASEDRPGCOMBAT_API UAbilityInputConfigData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AbilityInput1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AbilityInput2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AbilityInput3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AbilityInput4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ExecuteReadyAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* CancelReadyAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ConfirmTarget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* CancelConfirmedTarget;
};
