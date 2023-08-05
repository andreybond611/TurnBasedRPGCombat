// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API ARPGGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
	virtual void StartPlay() override;
};
