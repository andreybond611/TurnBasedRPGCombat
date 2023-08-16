// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "UI/HUD/HUDComponents/CharacterPortraitWidget.h"
#include "CharacterProgression/GameStat.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "UnrealFramework/RPGPlayerController.h"

void UCharacterPortraitWidget::Init(TScriptInterface<ITBBattleParticipant> Combatant)
{
	DisplayedCombatant = Combatant;
	Image->SetBrushFromTexture(Combatant->GetPortrait());

	Health = Combatant->Stats()->FindStat(SN_Health);
	if (!Health)
	{
		return;
	}

	Health->OnChange.AddUObject(this, &UCharacterPortraitWidget::OnHealthChange);
	OnHealthChange(Health->Get());
}

void UCharacterPortraitWidget::PlayCurrentTurnAnimation()
{
	PlayAnimation(CurrentTurnInSequence);
}

void UCharacterPortraitWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto* PlayerController = GetOwningPlayer<ARPGPlayerController>();
	if (ensure(PlayerController))
	{
		PlayerController->SetIsTargetAllowed(false);
	}
}

void UCharacterPortraitWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	auto* PlayerController = GetOwningPlayer<ARPGPlayerController>();
	if (ensure(PlayerController))
	{
		PlayerController->SetIsTargetAllowed(true);
	}
}

void UCharacterPortraitWidget::OnHealthChange(float Value)
{
	float HealthPercent = Value / Health->GetMaxValue();
	HealthBar->SetPercent(HealthPercent);
}
