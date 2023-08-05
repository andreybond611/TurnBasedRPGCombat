// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionPointsBarWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UGameStat;
/**
 * Shows number of action points
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UActionPointsBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Bind(UGameStat* InAPStat, UGameStat* InMaxAPStat);
	void RemoveBinding();

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ActionPointsBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* APText;

	FDelegateHandle OnAPChangeHandle;
	FDelegateHandle OnMaxAPChangeHandle;

	UPROPERTY()
	UGameStat* APStat;
	UPROPERTY()
	UGameStat* MaxAPStat;

	void OnMaxAPChange(float MaxAPValue);
	void OnAPChange(float APValue);
};
