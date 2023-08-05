// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndTurnWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UEndTurnWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
private:
	UPROPERTY(meta = (BindWidget))
	UButton* EndTurnButton;

	UFUNCTION()
	void EndTurn();
	
};
