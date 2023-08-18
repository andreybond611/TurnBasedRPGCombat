// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/RPGCharacter.h"
#include "Components/ActorComponent.h"
#include "EffectComponent.generated.h"

class UEffect;
class UConstantEffectMap;
/**
 * Applies and removes UEffects
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TURNBASEDRPGCOMBAT_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEffectComponent();

	/** Applies effect to owner */
	void AddEffect(UEffect* Effect);

	/** Removes effect from owner */
	void RemoveEffect(UEffect* Effect);
	void RemoveEffectClass(TSubclassOf<UEffect> EffectClass);

	/** Reduces every effect on one turn */
	void ReduceEffects();

	/** Called when owner starts turn */
	void StartTurn();

	template <typename EffectClass>
	bool HasEffect(TSubclassOf<UEffect> Class = EffectClass::StaticClass());
	bool HasEffect(TSubclassOf<UEffect> Class);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void OnRegister() override;

	void AddConstantEffects();

private:
	/** Applied effects */
	UPROPERTY(VisibleInstanceOnly)
	TArray<UEffect*> Effects;

	/** Owner character */
	UPROPERTY()
	ARPGCharacter* Character;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UConstantEffectMap> ConstantEffectMap;

	UEffect* FindSameClassEffect(UEffect* Effect);
};

template <typename EffectClass>
bool UEffectComponent::HasEffect(const TSubclassOf<UEffect> Class)
{
	return HasEffect(Class);
}

