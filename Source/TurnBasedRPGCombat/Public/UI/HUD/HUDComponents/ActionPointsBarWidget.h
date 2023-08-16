// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionPointsBarWidget.generated.h"

class UHorizontalBox;
class UActionPointWidget;
class UGameStat;
/**
 * Shows number of action points
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UActionPointsBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Bind(UGameStat* InAPStat);
	void RemoveBinding();

private:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ActionPointsBar;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActionPointWidget> ActionPointWidgetClass;

	FDelegateHandle OnAPChangeHandle;
	FDelegateHandle OnMaxAPChangeHandle;

	UPROPERTY()
	UGameStat* APStat;
	UPROPERTY()
	UGameStat* MaxAPStat;

	void OnMaxAPChange(float MaxAPValue);
	void OnAPChange(float APValue);
};
