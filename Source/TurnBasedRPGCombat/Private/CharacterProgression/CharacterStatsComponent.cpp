// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "CharacterProgression/CharacterStatsComponent.h"
#include "CharacterProgression/GameStat.h"
#include "Components/WidgetComponent.h"
#include "UI/FloatingNumbersWidget.h"

void UCharacterStatsComponent::InitializeStats()
{
	Super::InitializeStats();

	SetStatAsMaxStat(SN_Health, SN_MaxHealth);
	SetStatAsMaxStat(SN_ActionPoints, SN_MaxActionPoints);
	SetStatAsMaxStat(SN_PhysicalArmor, SN_MaxPhysicalArmor);
	SetStatAsMaxStat(SN_MagicArmor, SN_MaxMagicArmor);

	FindStat(SN_Health)->OnAddStat.AddUObject(this, &UCharacterStatsComponent::OnHeal);
}

void UCharacterStatsComponent::OnHeal(float HealedHealth)
{
	if (auto WidgetComponent = GetOwner()->FindComponentByClass<UWidgetComponent>())
	{
		if (auto FloatingNumbersWidget = Cast<UFloatingNumbersWidget>(WidgetComponent->GetWidget()))
		{
			FloatingNumbersWidget->AddFloatingNumber(HealedHealth, EFloatingNumberColor::Heal);
		}
	}
}
