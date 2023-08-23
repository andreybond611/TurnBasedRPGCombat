// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/Effects/HealEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Abilities/Effects/EffectComponent.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Components/CapsuleComponent.h"

UHealEffect::UHealEffect()
{
}

void UHealEffect::SpawnVisualEffect()
{
	SpawnedNiagara =
		UNiagaraFunctionLibrary::SpawnSystemAttached(VisualEffect, TargetActor->FindComponentByClass<UCapsuleComponent>(), NAME_None,
													 FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale, true);

	FTimerHandle EffectLifetimeHandle;
	GetWorld()->GetTimerManager().SetTimer(
		EffectLifetimeHandle, [this]() { RemoveVisualEffect(); }, VisualEffectLifetime, false);
}

void UHealEffect::ApplyTo(AActor* InActor)
{
	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(InActor))
	{
		GameplayTagHolder->AddTag(HealTag);
	}

	RemoveVisualEffect();
	SpawnVisualEffect();

	if (const auto Character = Cast<ARPGCharacter>(InActor))
	{
		Character->Heal(HealPerTurn);
	}

	UEffectComponent* EffectComponent = InActor->FindComponentByClass<UEffectComponent>();
	if (EffectComponent)
	{
		for (const TSubclassOf<UEffect> DebuffClass : EffectsToRemove)
		{
			EffectComponent->RemoveEffectClass(DebuffClass);
		}
	}
}

void UHealEffect::Remove()
{
	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		GameplayTagHolder->RemoveTag(HealTag);
	}
}

void UHealEffect::OnOwnerStartTurn()
{
	SpawnVisualEffect();

	if (const auto Character = Cast<ARPGCharacter>(TargetActor))
	{
		Character->Heal(HealPerTurn);
	}
}

void UHealEffect::RemoveVisualEffect()
{
	if (SpawnedNiagara)
	{
		SpawnedNiagara->DestroyInstance();
		SpawnedNiagara = nullptr;
	}
}
