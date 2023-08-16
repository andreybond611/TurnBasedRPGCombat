// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UActionPointsBarWidget;
class UTurnSequenceWidget;
class UHealthBarWidget;
class ARPGCharacter;
class ARPGPlayerController;
class UCanvasPanel;
class UProgressBar;
class UTextBlock;
/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(ARPGPlayerController* Player);

	void DisplayCharacter(ARPGCharacter* Character);
	void UndisplayCharacter(ARPGCharacter* Previous);

	void SetMouseOverCharacter(ARPGCharacter* Character) { MouseOverCharacter = Character; }
	UActionPointsBarWidget* GetAPBar() const { return ActionPointsBar; }

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	UPROPERTY(meta = (BindWidget))
	UActionPointsBarWidget* ActionPointsBar;

	UPROPERTY(meta = (BindWidget))
	UHealthBarWidget* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UHealthBarWidget* TargetHealthBar;
	UPROPERTY(meta = (BindWidget))
	UTurnSequenceWidget* TurnSequence;

	UFUNCTION()
	void OnControlledCharacterChanged(ARPGCharacter* Current, ARPGCharacter* Previous);

	UFUNCTION()
	void ShowTurnSequence(bool bPlayAnimation = true);
	UFUNCTION()
	void HideTurnSequence();
	UFUNCTION()
	void OnDisplayedCharacterEnteredBattle();

	UPROPERTY()
	ARPGPlayerController* OwningPlayer;
	UPROPERTY()
	ARPGCharacter* DisplayedCharacter;
	UPROPERTY()
	ARPGCharacter* ControlledCharacter;
	UPROPERTY()
	ARPGCharacter* MouseOverCharacter;
};
