// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/ShieldAbility.h"
#include "Characters/RPGCharacter.h"
#include "Abilities/Effects/Effect.h"
#include "Abilities/Effects/EffectComponent.h"

void UShieldAbility::StartAbility()
{
	Owner->PlayAnimMontage(AbilityMontage);
}

void UShieldAbility::AddToCharacter(ARPGCharacter* Character)
{
	Super::AddToCharacter(Character);
	OnAnimNotifyExecuted.AddDynamic(this, &UShieldAbility::ApplyEffect);
}

void UShieldAbility::RemoveFromCharacter(ARPGCharacter* Character)
{
	Super::RemoveFromCharacter(Character);
	OnAnimNotifyExecuted.RemoveDynamic(this, &UShieldAbility::ApplyEffect);
}

void UShieldAbility::ApplyEffect()
{
	if (auto ShieldEffect = NewObject<UEffect>(this, *AppliedEffectClass))
	{
		if (auto EffectComponent = GetTarget().Actor->FindComponentByClass<UEffectComponent>())
		{
			ShieldEffect->SetTurnCount(Turns);
			EffectComponent->AddEffect(ShieldEffect);
		}
	}

	EndAbility();
}
