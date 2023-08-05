// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/Effects/EffectComponent.h"
#include "Characters/RPGCharacter.h"
#include "Abilities/Effects/Effect.h"

UEffectComponent::UEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UEffect* UEffectComponent::FindSameClassEffect(UEffect* Effect)
{
	UEffect** SameClassEffect = Effects.FindByPredicate([Effect](UEffect* SearchedEffect) { return SearchedEffect->IsA(Effect->GetClass()); });
	return SameClassEffect ? *SameClassEffect : nullptr;
}

void UEffectComponent::AddEffect(UEffect* Effect)
{
	if (Effect->CanApplyTo(Character))
	{
		UEffect* SameClassEffect = FindSameClassEffect(Effect);
		if (SameClassEffect)
		{
			SameClassEffect->Stack(Effect);
		}
		else
		{
			Effect->TargetActor = Character;
			Effect->ApplyTo(Character);
			Effect->ApplyVisuals();
			Effects.Add(Effect);
		}
	}
}

void UEffectComponent::RemoveEffect(UEffect* Effect)
{
	if (Effect->GetTurnCount() <= 0 || Effect->CanRemove())
	{
		Effect->Remove();
		Effect->RemoveVisuals();
		Effects.Remove(Effect);
	}
}

void UEffectComponent::RemoveEffectClass(TSubclassOf<UEffect> EffectClass)
{
	UEffect** FoundEffect = Effects.FindByPredicate([EffectClass](UEffect* SearchedEffect) { return SearchedEffect->IsA(EffectClass); });
	if (FoundEffect)
	{
		RemoveEffect(*FoundEffect);
	}
}

void UEffectComponent::ReduceEffects()
{
	TArray<UEffect*> EffectsToRemove;
	for (UEffect* Effect : Effects)
	{
		Effect->ReduceTurn();
		if (Effect->GetTurnCount() <= 0)
		{
			EffectsToRemove.Add(Effect);
		}
	}

	for (UEffect* ToRemove : EffectsToRemove)
	{
		RemoveEffect(ToRemove);
	}
}

void UEffectComponent::StartTurn()
{
	for (UEffect* Effect : Effects)
	{
		Effect->OnOwnerStartTurn();
	}

	ReduceEffects();
}

bool UEffectComponent::HasEffect(TSubclassOf<UEffect> Class)
{
	for (UEffect* Effect : Effects)
	{
		if (Effect->IsA(Class))
		{
			return true;
		}
	}
	return false;
}

void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ARPGCharacter>(GetOwner());
}
