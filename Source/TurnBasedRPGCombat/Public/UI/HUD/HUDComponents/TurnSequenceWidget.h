// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/RPGCharacter.h"
#include "TurnSequenceWidget.generated.h"

class USpacer;
class UCharacterPortraitWidget;
class ATurnBasedBattle;
class UHorizontalBox;
/**
 *	Shows character in battle in order of their turns
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UTurnSequenceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(ATurnBasedBattle* Battle);

	ATurnBasedBattle* GetDisplayedBattle() const { return BoundBattle; }

private:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Portraits;
	UPROPERTY(meta = (BindWidget))
	USpacer* LeftSpacer;
	UPROPERTY(meta = (BindWidget))
	USpacer* RightSpacer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCharacterPortraitWidget> PortraitWidgetClass;

	UPROPERTY()
	ATurnBasedBattle* BoundBattle;
	UPROPERTY()
	TScriptInterface<ITBBattleParticipant> NextParticipant;

	void AddPortrait(TScriptInterface<ITBBattleParticipant> Combatant);
	void AddTurnQueue(TArray<TScriptInterface<ITBBattleParticipant>> CurrentTurnQueue);
	void AddSpacer();
	void FillPortraits();
	void RemoveFirstPortrait();
	UCharacterPortraitWidget* GetFirstPortrait();

	UFUNCTION()
	void OnNextCombatant(TScriptInterface<ITBBattleParticipant> Combatant);
	UFUNCTION()
	void OnAddNextTurnQueue(const TArray<TScriptInterface<ITBBattleParticipant>>& CurrentTurnQueue,
							const TArray<TScriptInterface<ITBBattleParticipant>>& NextTurnQueue);
};
