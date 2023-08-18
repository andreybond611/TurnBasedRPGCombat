// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "UI/HUD/HUDWidget.h"
#include "Blueprint/WidgetTree.h"
#include "ActorComponents/TurnBasedComponent.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "TurnBased/TurnBasedBattle.h"
#include "UI/HUD/HUDComponents/ActionPointsBarWidget.h"
#include "UI/HUD/HUDComponents/HealthBarWidget.h"
#include "UI/HUD/HUDComponents/TurnSequenceWidget.h"
#include "UnrealFramework/RPGPlayerController.h"

#define LOCTEXT_NAMESPACE "UI"

void UHUDWidget::Init(ARPGPlayerController* Player)
{
	OwningPlayer = Player;
	OwningPlayer->OnControlledCharacterChanged.AddDynamic(this, &UHUDWidget::OnControlledCharacterChanged);
	if (OwningPlayer->GetControlledCharacter())
	{
		DisplayCharacter(OwningPlayer->GetControlledCharacter());
	}
}

void UHUDWidget::DisplayCharacter(ARPGCharacter* Character)
{
	if (Character)
	{
		if (DisplayedCharacter)
		{
			UndisplayCharacter(DisplayedCharacter);
		}

		DisplayedCharacter = Character;

		UStatsComponent* Stats = Character->Stats();
		HealthBar->Bind(Stats);
		HealthBar->SetVisibility(ESlateVisibility::Visible);
		HealthBar->SetShowHealthNumbersOnlyOnMouseOver(true);

		ActionPointsBar->Bind(Stats->FindStat(SN_ActionPoints));
		ActionPointsBar->SetVisibility(ESlateVisibility::Visible);

		if (Character->IsInBattle())
		{
			if (Character->GetTurnBasedComponent()->GetCurrentBattle() != TurnSequence->GetDisplayedBattle())
			{
				ShowTurnSequence(false);
			}
		}
		Character->GetTurnBasedComponent()->OnEnterBattle.AddDynamic(this, &UHUDWidget::OnDisplayedCharacterEnteredBattle);
		Character->GetTurnBasedComponent()->OnExitBattle.AddDynamic(this, &UHUDWidget::HideTurnSequence);
	}
}

void UHUDWidget::UndisplayCharacter(ARPGCharacter* Previous)
{
	HealthBar->Unbind();
	HealthBar->SetVisibility(ESlateVisibility::Hidden);
	ActionPointsBar->RemoveBinding();
	ActionPointsBar->SetVisibility(ESlateVisibility::Hidden);

	Previous->GetTurnBasedComponent()->OnEnterBattle.RemoveDynamic(this, &UHUDWidget::OnDisplayedCharacterEnteredBattle);
	Previous->GetTurnBasedComponent()->OnExitBattle.RemoveDynamic(this, &UHUDWidget::HideTurnSequence);
}

void UHUDWidget::ShowTurnSequence(bool bPlayAnimation)
{
	ATurnBasedBattle* Battle = DisplayedCharacter->GetTurnBasedComponent()->GetCurrentBattle();
	if (ensure(Battle))
	{
		TurnSequence->SetVisibility(ESlateVisibility::Visible);
		TurnSequence->Init(Battle);
	}
}

void UHUDWidget::HideTurnSequence()
{
	TurnSequence->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDWidget::OnDisplayedCharacterEnteredBattle()
{
	ShowTurnSequence();
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MouseOverCharacter)
	{
		if (!TargetHealthBar->IsBound())
		{
			TargetHealthBar->Bind(MouseOverCharacter->Stats());
			TargetHealthBar->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (TargetHealthBar->IsBound())
		{
			TargetHealthBar->SetVisibility(ESlateVisibility::Hidden);
			TargetHealthBar->Unbind();
		}
	}
}

void UHUDWidget::OnControlledCharacterChanged(ARPGCharacter* Current, ARPGCharacter* Previous)
{
	DisplayCharacter(Current);
}

#undef LOCTEXT_NAMESPACE
