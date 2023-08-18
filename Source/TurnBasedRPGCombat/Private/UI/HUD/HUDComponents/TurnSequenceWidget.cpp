// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/HUD/HUDComponents/TurnSequenceWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/RPGCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "TurnBased/TurnBasedBattle.h"
#include "UI/HUD/HUDComponents/CharacterPortraitWidget.h"

void UTurnSequenceWidget::AddPortrait(const TScriptInterface<ITBBattleParticipant> Combatant)
{
	auto* CharacterPortrait = CreateWidget<UCharacterPortraitWidget>(this, *PortraitWidgetClass);
	CharacterPortrait->Init(Combatant);
	Portraits->AddChildToHorizontalBox(CharacterPortrait);
}

void UTurnSequenceWidget::AddTurnQueue(TArray<TScriptInterface<ITBBattleParticipant>> CurrentTurnQueue)
{
	for (int32 CharacterCount = CurrentTurnQueue.Num() - 1; CharacterCount >= 0; --CharacterCount)
	{
		AddPortrait(CurrentTurnQueue[CharacterCount]);
	}
}

void UTurnSequenceWidget::AddSpacer()
{
	auto* BetweenQueuesSpacer = WidgetTree->ConstructWidget<USpacer>();
	BetweenQueuesSpacer->SetSize({20.f, 0.f});
	Portraits->AddChildToHorizontalBox(BetweenQueuesSpacer);
}

void UTurnSequenceWidget::FillPortraits()
{
	const TArray<TScriptInterface<ITBBattleParticipant>> CurrentTurnQueue = BoundBattle->GetCurrentTurnQueue();
	const TArray<TScriptInterface<ITBBattleParticipant>> NextTurnQueue = BoundBattle->GetNextTurnQueue();

	Portraits->ClearChildren();

	UHorizontalBoxSlot* HBSlot = Portraits->AddChildToHorizontalBox(LeftSpacer);
	HBSlot->SetSize(ESlateSizeRule::Fill);

	AddTurnQueue(CurrentTurnQueue);
	AddSpacer();
	AddTurnQueue(NextTurnQueue);

	HBSlot = Portraits->AddChildToHorizontalBox(RightSpacer);
	HBSlot->SetSize(ESlateSizeRule::Fill);

	GetFirstPortrait()->PlayCurrentTurnAnimation();
}

void UTurnSequenceWidget::RemoveFirstPortrait()
{
	if (Portraits->GetChildAt(1) && Portraits->GetChildAt(1)->IsA<UCharacterPortraitWidget>())
	{
		Portraits->RemoveChildAt(1);

		// also remove a spacer if it's next
		if (Portraits->GetChildAt(1) && Portraits->GetChildAt(1)->IsA<USpacer>())
		{
			Portraits->RemoveChildAt(1);
		}
	}
}

UCharacterPortraitWidget* UTurnSequenceWidget::GetFirstPortrait()
{
	if (Portraits->GetChildAt(1) && Portraits->GetChildAt(1)->IsA<UCharacterPortraitWidget>())
	{
		return Cast<UCharacterPortraitWidget>(Portraits->GetChildAt(1));
	}
	return nullptr;
}

void UTurnSequenceWidget::OnNextCombatant(const TScriptInterface<ITBBattleParticipant> Combatant)
{
	if (NextParticipant != nullptr)
	{
		RemoveFirstPortrait();
		GetFirstPortrait()->PlayCurrentTurnAnimation(); //-V522
	}
	NextParticipant = Combatant;
}

void UTurnSequenceWidget::OnAddNextTurnQueue(const TArray<TScriptInterface<ITBBattleParticipant>>& CurrentTurnQueue,
											 const TArray<TScriptInterface<ITBBattleParticipant>>& NextTurnQueue)
{
	Portraits->RemoveChildAt(Portraits->GetChildrenCount() - 1);

	AddSpacer();

	AddTurnQueue(NextTurnQueue);

	UHorizontalBoxSlot* HBSlot = Portraits->AddChildToHorizontalBox(RightSpacer);
	HBSlot->SetSize(ESlateSizeRule::Fill);
}

void UTurnSequenceWidget::Init(ATurnBasedBattle* Battle)
{
	if (ensure(Battle))
	{
		BoundBattle = Battle;
		FillPortraits();

		BoundBattle->OnSetNextParticipant.AddDynamic(this, &UTurnSequenceWidget::OnNextCombatant);
		BoundBattle->OnAddNextTurnQueue.AddDynamic(this, &UTurnSequenceWidget::OnAddNextTurnQueue);
	}
}
