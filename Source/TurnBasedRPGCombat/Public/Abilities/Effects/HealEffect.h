// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Effects/Effect.h"
#include "HealEffect.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
/**
 * Heals every turn and removes some effects when applied
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UHealEffect : public UEffect
{
	GENERATED_BODY()
	UHealEffect();
	void SpawnVisualEffect();
public:
	virtual void ApplyTo(AActor* InActor) override;
	virtual void Remove() override;
	virtual void OnOwnerStartTurn() override;

private:
	UPROPERTY(EditAnywhere)
	float HealPerTurn = 26.f;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* VisualEffect;
	UPROPERTY(EditAnywhere)
	float VisualEffectLifetime = 1.f;
	UPROPERTY(EditAnywhere)
	FGameplayTag HealTag;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UEffect>> EffectsToRemove;

	UPROPERTY()
	UNiagaraComponent* SpawnedNiagara;

	void RemoveVisualEffect();
	
};
