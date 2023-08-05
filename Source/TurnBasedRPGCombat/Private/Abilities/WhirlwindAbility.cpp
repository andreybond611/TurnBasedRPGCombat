// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/WhirlwindAbility.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Utility/TurnBasedUtility.h"

void UWhirlwindAbility::StartAbility()
{
	float AnimationLength = Owner->PlayAnimMontage(AbilityMontage);

	FTimerHandle AnimationEndHandle;
	GetWorld()->GetTimerManager().SetTimer(AnimationEndHandle, this, &UWhirlwindAbility::EndAbility, AnimationLength);
}

void UWhirlwindAbility::AddToCharacter(ARPGCharacter* Character)
{
	Super::AddToCharacter(Character);

	OnAnimNotifyExecuted.AddDynamic(this, &UWhirlwindAbility::HitEveryoneAround);
}

void UWhirlwindAbility::RemoveFromCharacter(ARPGCharacter* Character)
{
	Super::RemoveFromCharacter(Character);

	OnAnimNotifyExecuted.RemoveDynamic(this, &UWhirlwindAbility::HitEveryoneAround);
}


void UWhirlwindAbility::HitEveryoneAround()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner, WhirlwindEffect, Owner->GetActorLocation(), Owner->GetActorRotation());

	for (AActor* Actor : GetTarget().MultipleActors)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Actor, Impact, Actor->GetActorLocation(), Actor->GetActorRotation());

		if (auto Damageable = Cast<IDamageable>(Actor))
		{
			float DamageNumber = Owner->Stats()->Get(SN_Damage) * DamageMultiplier;
			EHitDirection HitDirection = UTurnBasedUtility::FindHitDirection(Owner, Actor);

			FDamage Damage;
			Damage.DamageNumber = DamageNumber;
			Damage.HitDirection = HitDirection;
			Damage.DamageType = EDamageType::Physical;

			Damageable->GetDamaged(Damage);
		}
	}
}
