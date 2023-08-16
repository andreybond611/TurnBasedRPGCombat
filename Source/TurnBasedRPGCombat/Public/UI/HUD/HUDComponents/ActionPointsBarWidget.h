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

	/** Shows to player how much AP gonna be spent on an action */
	void SetSpendingIndication(int32 APCount);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
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

	bool bSpendingIndication = false;
	UPROPERTY()
	TArray<UActionPointWidget*> ActionPointWidgets;

	void OnMaxAPChange(float MaxAPValue);
	void OnAPChange(float APValue);
};
