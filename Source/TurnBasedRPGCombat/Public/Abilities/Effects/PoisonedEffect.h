// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Effects/Effect.h"
#include "PoisonedEffect.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UPoisonedEffect : public UEffect
{
	GENERATED_BODY()
	UPoisonedEffect();

public:
	virtual void ApplyTo(AActor* InActor) override;
	virtual void Remove() override;

	virtual void ApplyVisuals() override;
	virtual void RemoveVisuals() override;

	virtual void OnOwnerStartTurn() override;

	UPROPERTY(EditAnywhere)
	float DamagePerTurn = 11.f;

private:
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PoisonCharacterEffect;

	UPROPERTY()
	UNiagaraComponent* SpawnedVisual;
};
