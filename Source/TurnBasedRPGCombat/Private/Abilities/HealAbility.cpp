// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/HealAbility.h"

#include "Abilities/Effects/Effect.h"
#include "Abilities/Effects/EffectComponent.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "Characters/RPGCharacter.h"

UHealAbility::UHealAbility()
{
	
}

void UHealAbility::StartAbility()
{
	Owner->PlayAnimMontage(AbilityMontage);
	
}

void UHealAbility::AddToCharacter(ARPGCharacter* Character)
{
	Super::AddToCharacter(Character);

	OnAnimNotifyExecuted.AddDynamic(this, &UHealAbility::ApplyEffect);
}

void UHealAbility::RemoveFromCharacter(ARPGCharacter* Character)
{
	Super::RemoveFromCharacter(Character);

	OnAnimNotifyExecuted.RemoveDynamic(this, &UHealAbility::ApplyEffect);
}

bool UHealAbility::IsDetected(AActor* Target)
{
	if (auto* TeamInterface = Cast<IGenericTeamAgentInterface>(Target))
	{
		if (TeamInterface->GetTeamAttitudeTowards(*Owner) == ETeamAttitude::Hostile)
		{
			if (auto* TagHolder = Cast<IGameplayTagHolder>(Target))
			{
				return TagHolder->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("ConstantEffect.Undead"));
			}
		}

		return TeamInterface->GetTeamAttitudeTowards(*Owner) == ETeamAttitude::Friendly;
	}

	return false;
}

void UHealAbility::ApplyEffect()
{
	if (const auto HealEffect = NewObject<UEffect>(this, *AppliedEffectClass))
	{
		if (const auto EffectComponent = GetTarget().Actor->FindComponentByClass<UEffectComponent>())
		{
			EffectComponent->AddEffect(HealEffect);
		}
	}
	
	EndAbility();
}
