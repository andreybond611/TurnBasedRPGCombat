// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/ReadiableAbility.h"

#include "NiagaraComponent.h"
#include "Characters/RPGCharacter.h"

void UReadiableAbility::ReadyAbility()
{
	ApplyVisualEffects();
}


void UReadiableAbility::ApplyVisualEffects()
{
	if (Owner)
	{
		if (GroundEffectClass)
		{
			Owner->GetMagicCircleComponent()->SetChildActorClass(*GroundEffectClass);
		}
		if (CascadeParticle)
		{
			Owner->GetParticleSystemComponent()->SetTemplate(CascadeParticle);
		}
		if (NiagaraParticle)
		{
			Owner->GetBodyEffectComponent()->SetAsset(NiagaraParticle);
		}
	}
}

void UReadiableAbility::HideVisualEffects()
{
	if (Owner)
	{
		if (GroundEffectClass)
		{
			Owner->GetMagicCircleComponent()->SetChildActorClass(nullptr);
		}
		if (CascadeParticle)
		{
			Owner->GetParticleSystemComponent()->SetTemplate(nullptr);
		}
		if (NiagaraParticle)
		{
			Owner->GetBodyEffectComponent()->SetAsset(nullptr);
		}
	}
}

bool UReadiableAbility::CanBeReadied()
{
	return RemainingCooldown <= 0.f;
}

void UReadiableAbility::CancelReadyAbility()
{
	HideVisualEffects();
}