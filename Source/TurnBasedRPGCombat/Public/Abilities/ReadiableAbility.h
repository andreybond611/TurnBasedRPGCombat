// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "ReadiableAbility.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UReadiableAbility : public UAbility
{
	GENERATED_BODY()

public:
	virtual void ReadyAbility();
	virtual bool CanBeReadied();
	virtual void CancelReadyAbility();
	FAbilityStateAnimations GetStateAnimations() const { return {Start, Ready, Cancel}; }

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GroundEffectClass;
	UPROPERTY(EditAnywhere)
	UParticleSystem* CascadeParticle;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraParticle;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* Start;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* Ready;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* Cancel;
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* AbilityMontage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEffect> AppliedEffectClass;

	void ApplyVisualEffects();
	void HideVisualEffects();
};
