// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_TargetActor.generated.h"

/**
 * Gets context from TargetActor
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UEnvQueryContext_TargetActor : public UEnvQueryContext
{
	GENERATED_BODY()
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
