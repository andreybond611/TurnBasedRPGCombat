// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/ShinbiDashAbility.h"

#include "Abilities/Effects/EffectComponent.h"
#include "Characters/RPGCharacter.h"
#include "Abilities/Effects/StunEffect.h"
#include "CharacterProgression/StatsComponent.h"
#include "Utility/TurnBasedUtility.h"

void UShinbiDashAbility::AddToCharacter(ARPGCharacter* Character)
{
	Super::AddToCharacter(Character);

	OnAnimNotifyExecuted.AddDynamic(this, &UShinbiDashAbility::TeleportToTarget);
}

void UShinbiDashAbility::RemoveFromCharacter(ARPGCharacter* Character)
{
	OnAnimNotifyExecuted.RemoveDynamic(this, &UShinbiDashAbility::TeleportToTarget);
}

void UShinbiDashAbility::StartAbility()
{
	PreviousLocation = Owner->GetActorLocation();
	Owner->PlayAnimMontage(AbilityMontage);
}

void UShinbiDashAbility::TeleportToTarget()
{
	const FTarget Target = GetTarget();
	Owner->SetActorLocation(Target.Location);

	TArray<AActor*> Actors = Target.MultipleActors;
	for (AActor* Actor : Actors)
	{
		auto* StunEffect = NewObject<UStunEffect>(this, *AppliedEffectClass);
		StunEffect->SetTurnCount(TurnsOfEffect);
		if (const auto EffectComponent = Actor->FindComponentByClass<UEffectComponent>())
		{
			EffectComponent->AddEffect(StunEffect);
		}

		if (const auto Damageable = Cast<IDamageable>(Actor))
		{
			const float DamageNumber = Owner->Stats()->Get(SN_Damage) * DamageMultiplier;
			const EHitDirection HitDirection = UTurnBasedUtility::FindHitDirection(PreviousLocation, Actor);

			FDamage Damage;
			Damage.DamageNumber = DamageNumber;
			Damage.HitDirection = HitDirection;
			Damage.DamageType = EDamageType::Physical;

			Damageable->GetDamaged(Damage);
		}
	}

	EndAbility();
}
