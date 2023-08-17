// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Effects/Effect.h"
#include "UndeadEffect.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UUndeadEffect : public UEffect
{
	GENERATED_BODY()
	UUndeadEffect();
public:
	virtual void ApplyTo(AActor* InActor) override;
	virtual void Remove() override;
};
