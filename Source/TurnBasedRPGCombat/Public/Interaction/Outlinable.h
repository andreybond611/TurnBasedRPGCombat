// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Outlinable.generated.h"

enum class EOutlineColor : FPlatformTypes::uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOutlinable : public UInterface
{
	GENERATED_BODY()
};

class TURNBASEDRPGCOMBAT_API IOutlinable
{
	GENERATED_BODY()

public:
	virtual void EnableOutline(EOutlineColor Color);
	virtual void DisableOutline();
};
