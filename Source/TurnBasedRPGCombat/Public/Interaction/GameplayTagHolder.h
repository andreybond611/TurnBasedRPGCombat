// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "UObject/Interface.h"
#include "GameplayTagHolder.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameplayTagHolder : public UGameplayTagAssetInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASEDRPGCOMBAT_API IGameplayTagHolder : public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddTag(const FGameplayTag& Tag) = 0;
	virtual void RemoveTag(const FGameplayTag& Tag) = 0;
};
