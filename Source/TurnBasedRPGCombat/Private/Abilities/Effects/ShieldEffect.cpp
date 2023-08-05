// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/Effects/ShieldEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Kismet/GameplayStatics.h"

UShieldEffect::UShieldEffect()
{
	
}

void UShieldEffect::ApplyTo(AActor* InActor)
{
	Super::ApplyTo(InActor);

	if (auto GameplayTagHolder = Cast<IGameplayTagHolder>(InActor))
	{
		GameplayTagHolder->AddTag(FGameplayTag::RequestGameplayTag("Effect.Buff.Shield"));
	}

	if (auto Stats = InActor->FindComponentByClass<UStatsComponent>())
	{
		Stats->Add(SN_MaxPhysicalArmor, ArmorToRaise);
		Stats->Add(SN_PhysicalArmor, ArmorToRaise);
	}
}

void UShieldEffect::Remove()
{
	Super::Remove();

	if (auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		GameplayTagHolder->RemoveTag(FGameplayTag::RequestGameplayTag("Effect.Buff.Shield"));
	}

	if (auto Stats = TargetActor->FindComponentByClass<UStatsComponent>())
	{
		Stats->Remove(SN_MaxPhysicalArmor, ArmorToRaise);
	}
}

void UShieldEffect::ApplyVisuals()
{
	SpawnedActorEffect = GetWorld()->SpawnActor(*ShieldEffectClass);
	FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetIncludingScale;
	SpawnedActorEffect->AttachToActor(TargetActor, Rules);
}

void UShieldEffect::RemoveVisuals()
{
	FDetachmentTransformRules Rules = FDetachmentTransformRules::KeepRelativeTransform;
	SpawnedActorEffect->DetachFromActor(Rules);
	SpawnedActorEffect->Destroy();
	SpawnedActorEffect = nullptr;
}
