// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DynamicObstacle.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDynamicObstacle : public UInterface
{
	GENERATED_BODY()
};

/**
 * Disabling dynamic obstacles gives more direct path, but pawns still should be able to go
 * around it, so we enable or disable dynamic obstacles when pawns go to or around a different pawn
 */
class TURNBASEDRPGCOMBAT_API IDynamicObstacle
{
	GENERATED_BODY()

public:
	virtual void EnableDynamicObstacle();
	virtual void DisableDynamicObstacle();
};
