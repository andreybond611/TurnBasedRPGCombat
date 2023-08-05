// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/DamageNumbersWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/FloatingDamageNumberWidget.h"
#include "Utility/TurnBasedTypes.h"

void UDamageNumbersWidget::AddDamageNumber(int32 Damage, FColor Color)
{
	FVector2D PanelSize = DamagePanel->GetDesiredSize();

	auto DamageNumber = CreateWidget<UFloatingDamageNumberWidget>(this, *FloatingDamageWidgetClass);
	UCanvasPanelSlot* PanelSlot = DamagePanel->AddChildToCanvas(DamageNumber);
	PanelSlot->SetPosition({PanelSize.X * 0.5f, PanelSize.Y});
	DamageNumber->Init(Damage, Color);
}

void UDamageNumbersWidget::AddDamageNumber(int32 Damage, EDamageColor DamageType)
{
	FColor DamageColor = GetColorFromDamageType(DamageType);
	AddDamageNumber(Damage, DamageColor);
}

FColor UDamageNumbersWidget::GetColorFromDamageType(EDamageColor DamageType) const
{
	switch (DamageType)
	{
	case EDamageColor::Physical: return PhysicalDamageColor;
	case EDamageColor::Fire: return FireDamageColor;
	case EDamageColor::Earth: return EarthDamageColor;
	case EDamageColor::Poison: return PoisonDamageColor;
	case EDamageColor::Ice: return IceDamageColor;
	case EDamageColor::PhysicalArmor: return PhysicalArmorDamageColor;
	case EDamageColor::MagicArmor: return MagicArmorDamageColor;
	default: checkNoEntry(); return FColor::Black;
	}
}
