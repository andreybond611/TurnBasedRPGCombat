// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/HUD/HUDComponents/EndTurnWidget.h"
#include "ActorComponents/TurnBasedComponent.h"
#include "Characters/RPGCharacter.h"
#include "Components/Button.h"
#include "UnrealFramework/RPGPlayerController.h"

void UEndTurnWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EndTurnButton->OnClicked.AddDynamic(this, &UEndTurnWidget::EndTurn);
}

void UEndTurnWidget::EndTurn()
{
	auto* PlayerController = GetOwningPlayer<ARPGPlayerController>();
	if (PlayerController)
	{
		if (ARPGCharacter* ControlledCharacter = PlayerController->GetControlledCharacter())
		{
			ControlledCharacter->GetTurnBasedComponent()->EndTurn();
		}
	}
}

void UEndTurnWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto* PlayerController = GetOwningPlayer<ARPGPlayerController>();
	if (ensure(PlayerController))
	{
		PlayerController->SetIsTargetAllowed(false);
	}
}

void UEndTurnWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	auto* PlayerController = GetOwningPlayer<ARPGPlayerController>();
	if (ensure(PlayerController))
	{
		PlayerController->SetIsTargetAllowed(true);
	}
}
