// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterPortraitWidget.generated.h"

class ITBBattleParticipant;
class ARPGCharacter;
class UBorder;
class UProgressBar;
class UImage;
class UGameStat;
/**
 *	Shows character image and their health
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UCharacterPortraitWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(TScriptInterface<ITBBattleParticipant> Combatant);
	void PlayCurrentTurnAnimation();

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UBorder* PortraitBorder;
	UPROPERTY(meta = (BindWidget))
	UImage* Image;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CurrentTurnInSequence;

	UPROPERTY()
	UGameStat* Health;
	UPROPERTY()
	TScriptInterface<ITBBattleParticipant> DisplayedCombatant;

	void OnHealthChange(float Value);
};
