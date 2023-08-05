// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/HUD/HUDComponents/HealthBarWidget.h"
#include "CharacterProgression/GameStat.h"
#include "CharacterProgression/StatsComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/ProgressBarBinding.h"

void UHealthBarWidget::Bind(UStatsComponent* StatsToBind)
{
	if (ensure(StatsToBind))
	{
		UGameStat* HealthStat = StatsToBind->FindStat(SN_Health);
		UGameStat* PhysicalArmorStat = StatsToBind->FindStat(SN_PhysicalArmor);
		UGameStat* MagicArmorStat = StatsToBind->FindStat(SN_MagicArmor);

		HealthBinding->Bind(HealthStat, HealthProgressBar, HealthText);
		PhysicalArmorBinding->Bind(PhysicalArmorStat, PhysicalArmorProgressBar, PhysicalArmorText);
		MagicArmorBinding->Bind(MagicArmorStat, MagicArmorProgressBar, MagicArmorText);
	}
}

void UHealthBarWidget::Unbind()
{
	HealthBinding->Unbind();
	PhysicalArmorBinding->Unbind();
	MagicArmorBinding->Unbind();
}

bool UHealthBarWidget::IsBound() const
{
	return HealthBinding->IsBound();
}

void UHealthBarWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (bShowHealthNumberOnlyOnMouseOver)
	{
		HealthText->SetVisibility(ESlateVisibility::Visible);
		PhysicalArmorText->SetVisibility(ESlateVisibility::Visible);
		MagicArmorText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UHealthBarWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (bShowHealthNumberOnlyOnMouseOver)
	{
		HealthText->SetVisibility(ESlateVisibility::Hidden);
		PhysicalArmorText->SetVisibility(ESlateVisibility::Hidden);
		MagicArmorText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHealthBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HealthBinding = NewObject<UProgressBarBinding>();
	PhysicalArmorBinding = NewObject<UProgressBarBinding>();
	MagicArmorBinding = NewObject<UProgressBarBinding>();
}

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bShowHealthNumberOnlyOnMouseOver ? HealthText->SetVisibility(ESlateVisibility::Hidden) : HealthText->SetVisibility(ESlateVisibility::Visible);
}

