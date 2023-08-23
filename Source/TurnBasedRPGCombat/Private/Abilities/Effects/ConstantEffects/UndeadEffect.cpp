// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/Effects/ConstantEffects/UndeadEffect.h"

#include "CharacterProgression/GameStat.h"
#include "CharacterProgression/StatsComponent.h"
#include "Utility/TurnBasedTypes.h"

UUndeadEffect::UUndeadEffect()
{
	bInfinite = true;
}

void UUndeadEffect::ApplyTo(AActor* InActor)
{
	if (auto* Stats = InActor->FindComponentByClass<UStatsComponent>())
	{
		Stats->SetConstant(SN_PoisonResistance, 200.f);
		Stats->SetConstant(SN_HealResistance, 0.f);
	}
}

void UUndeadEffect::Remove()
{
	if (auto* Stats = TargetActor->FindComponentByClass<UStatsComponent>())
	{
		UGameStat* PoisonResistanceStat = Stats->FindStat(SN_PoisonResistance);
		UGameStat* HealResistanceStat = Stats->FindStat(SN_HealResistance);

		PoisonResistanceStat->RemoveConstness();
		HealResistanceStat->RemoveConstness();

		PoisonResistanceStat->Remove(200.f);
		HealResistanceStat->Add(200.f);
	}
}
