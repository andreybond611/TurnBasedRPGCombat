// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/Effects/EffectComponent.h"
#include "Characters/RPGCharacter.h"
#include "Abilities/Effects/Effect.h"
#include "Abilities/Effects/ConstantEffects/ConstantEffectMap.h"
#include "Utility/TurnBasedUtility.h"

UEffectComponent::UEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UEffect* UEffectComponent::FindSameClassEffect(UEffect* Effect)
{
	Effects.Remove(nullptr);
	UEffect** SameClassEffect = Effects.FindByPredicate([Effect](UEffect* SearchedEffect) { return SearchedEffect->IsA(Effect->GetClass()); });
	return SameClassEffect ? *SameClassEffect : nullptr;
}

void UEffectComponent::AddEffect(UEffect* Effect)
{
	if (Effect->CanApplyTo(GetOwner()))
	{
		UEffect* SameClassEffect = FindSameClassEffect(Effect);
		if (SameClassEffect)
		{
			SameClassEffect->Stack(Effect);
		}
		else
		{
			Effect->TargetActor = GetOwner();
			Effect->ApplyTo(GetOwner());
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

bool UEffectComponent::HasEffect(const TSubclassOf<UEffect> Class)
{
	for (const UEffect* Effect : Effects)
	{
		if (Effect->IsA(Class))
		{
			return true;
		}
	}
	return false;
}

void UEffectComponent::AddConstantEffects()
{
	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(GetOwner()))
	{
		FGameplayTagContainer TagContainer;
		GameplayTagHolder->GetOwnedGameplayTags(TagContainer);

		const auto EffectMap = GetDefault<UConstantEffectMap>(ConstantEffectMap);

		for (FGameplayTag Tag : TagContainer)
		{
			if (EffectMap->ConstantEffectClasses.Contains(Tag))
			{
				TSubclassOf<UEffect> EffectClass = EffectMap->ConstantEffectClasses[Tag];
				const auto NewConstantEffect = NewObject<UEffect>(this, EffectClass);
				if (ensure(NewConstantEffect))
				{
					AddEffect(NewConstantEffect);
				}
			}
		}
	}
}

void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UEffectComponent::PostInitProperties()
{
	Super::PostInitProperties();

}

void UEffectComponent::OnRegister()
{
	Super::OnRegister();

	const FString Flags = UTurnBasedUtility::ObjectFlagToString(GetFlags());
	UE_LOG(LogTemp, Warning, TEXT("Name: %s, Flags: %s"), *GetName(), *Flags);

	if (!HasAllFlags(RF_ClassDefaultObject | RF_AllocatedInSharedPage))
	{
		AddConstantEffects();
	}
}
