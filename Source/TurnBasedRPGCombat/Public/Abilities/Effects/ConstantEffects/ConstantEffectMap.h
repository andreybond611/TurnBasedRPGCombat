// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ConstantEffectMap.generated.h"

class UEffect;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASEDRPGCOMBAT_API UConstantEffectMap : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, TSubclassOf<UEffect>> ConstantEffectClasses;

};
