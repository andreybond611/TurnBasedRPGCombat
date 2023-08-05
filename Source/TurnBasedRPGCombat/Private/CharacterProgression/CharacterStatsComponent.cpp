// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "CharacterProgression/CharacterStatsComponent.h"
#include "CharacterProgression/GameStat.h"

void UCharacterStatsComponent::InitializeStats()
{
	Super::InitializeStats();

	SetStatAsMaxStat(SN_Health, SN_MaxHealth);
	SetStatAsMaxStat(SN_ActionPoints, SN_MaxActionPoints);
	SetStatAsMaxStat(SN_PhysicalArmor, SN_MaxPhysicalArmor);
	SetStatAsMaxStat(SN_MagicArmor, SN_MaxMagicArmor);
}
