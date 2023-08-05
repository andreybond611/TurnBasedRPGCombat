// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Effect.h"
#include "GameplayTagContainer.h"
#include "StunEffect.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UStunEffect : public UEffect
{
	GENERATED_BODY()
	UStunEffect();

protected:
	virtual void ApplyTo(AActor* InActor) override;
	virtual bool CanApplyTo(AActor* InActor) override;
	virtual void Remove() override;

	FGameplayTag ImmunityToStunTag;

private:
	virtual void ApplyVisuals() override;
	virtual void RemoveVisuals() override;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* StunVisual;

	UPROPERTY()
	UNiagaraComponent* SpawnedSystem;
};
