// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/FloatingNumbersWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/FloatingNumberWidget.h"
#include "Utility/TurnBasedTypes.h"

void UFloatingNumbersWidget::AddFloatingNumber(int32 Number, FColor Color)
{
	FVector2D PanelSize = FloatingNumbersPanel->GetDesiredSize();

	auto FloatingNumberWidget = CreateWidget<UFloatingNumberWidget>(this, *FloatingDamageWidgetClass);
	UCanvasPanelSlot* PanelSlot = FloatingNumbersPanel->AddChildToCanvas(FloatingNumberWidget);
	PanelSlot->SetPosition({PanelSize.X * 0.5f, PanelSize.Y});
	FloatingNumberWidget->Init(Number, Color);
}

void UFloatingNumbersWidget::AddFloatingNumber(int32 Number, EFloatingNumberColor Color)
{
	FColor FloatingNumberColor = GetColorFromDamageType(Color);
	AddFloatingNumber(Number, FloatingNumberColor);
}

FColor UFloatingNumbersWidget::GetColorFromDamageType(EFloatingNumberColor DamageType) const
{
	switch (DamageType)
	{
	case EFloatingNumberColor::Physical: return PhysicalDamageColor;
	case EFloatingNumberColor::Fire: return FireDamageColor;
	case EFloatingNumberColor::Earth: return EarthDamageColor;
	case EFloatingNumberColor::Poison: return PoisonDamageColor;
	case EFloatingNumberColor::Ice: return IceDamageColor;
	case EFloatingNumberColor::PhysicalArmor: return PhysicalArmorDamageColor;
	case EFloatingNumberColor::MagicArmor: return MagicArmorDamageColor;
	case EFloatingNumberColor::Heal: return HealColor;
	default: checkNoEntry(); return FColor::Black;
	}
}

void UFloatingNumbersWidget::AddFloatingMessage(const FText& Message, FColor Color)
{
	FVector2D PanelSize = FloatingNumbersPanel->GetDesiredSize();

	auto FloatingNumberWidget = CreateWidget<UFloatingNumberWidget>(this, *FloatingDamageWidgetClass);
	UCanvasPanelSlot* PanelSlot = FloatingNumbersPanel->AddChildToCanvas(FloatingNumberWidget);
	PanelSlot->SetPosition({ PanelSize.X * 0.5f, PanelSize.Y });
	FloatingNumberWidget->Init(Message, Color);
}
