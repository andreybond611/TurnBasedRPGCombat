// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/Effects/SlowedEffect.h"

#include "ActorComponents/RPGMovementComponent.h"
#include "CharacterProgression/GameStat.h"
#include "Utility/TurnBasedTypes.h"
#include "CharacterProgression/StatsComponent.h"
#include "Interaction/GameplayTagHolder.h"

void USlowedEffect::ApplyTo(AActor* InActor)
{
	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(InActor))
	{
		GameplayTagHolder->AddTag(FGameplayTag::RequestGameplayTag("Effect.Debuff.Slowed"));
	}

	if (const auto Stats = InActor->FindComponentByClass<UStatsComponent>())
	{
		Stats->AddMultiplier(SN_MaxWalkPerAP, -0.5f);
	}

	if (const auto MovementComponent = InActor->FindComponentByClass<URPGMovementComponent>())
	{
		MovementComponent->GetWalkSpeedStat()->AddMultiplier(MovementMultiplier);
	}
}

void USlowedEffect::Remove()
{
	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		GameplayTagHolder->RemoveTag(FGameplayTag::RequestGameplayTag("Effect.Debuff.Slowed"));
	}

	if (const auto Stats = TargetActor->FindComponentByClass<UStatsComponent>())
	{
		Stats->AddMultiplier(SN_MaxWalkPerAP, 0.5f);
	}

	if (const auto MovementComponent = TargetActor->FindComponentByClass<URPGMovementComponent>())
	{
		MovementComponent->GetWalkSpeedStat()->AddMultiplier(-MovementMultiplier);
	}
}

bool USlowedEffect::CanRemove()
{
	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		if (GameplayTagHolder->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Surface.Oil")))
		{
			return false;
		}
	}
	return true;
}
