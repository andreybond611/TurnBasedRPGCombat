// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Effect.h"
#include "NiagaraComponent.h"
#include "OnFireEffect.generated.h"

class UNiagaraSystem;
/**
 * Target is on fire and takes damage every turn
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UOnFireEffect : public UEffect
{
	GENERATED_BODY()
	UOnFireEffect();
public:
	virtual void ApplyTo(AActor* InActor) override;
	virtual void Remove() override;

	virtual void ApplyVisuals() override;
	virtual void RemoveVisuals() override;

	virtual void OnOwnerStartTurn() override;

	UPROPERTY(EditAnywhere)
	float DamagePerTurn = 12.f;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* FireVisual;

	UPROPERTY()
	UNiagaraComponent* SpawnedVisual;
};
