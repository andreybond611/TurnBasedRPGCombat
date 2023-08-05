// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Effect.h"
#include "ShieldEffect.generated.h"

class UNiagaraSystem;
/**
 * Raises up physical armor stat
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UShieldEffect : public UEffect
{
	GENERATED_BODY()
	UShieldEffect();
public:
	virtual void ApplyTo(AActor* InActor) override;
	virtual void Remove() override;
	virtual void ApplyVisuals() override;
	virtual void RemoveVisuals() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ShieldEffectClass;
	UPROPERTY(EditAnywhere, Category = "Effect Parameters")
	float ArmorToRaise = 50.f;

	UPROPERTY()
	AActor* SpawnedActorEffect;
};
