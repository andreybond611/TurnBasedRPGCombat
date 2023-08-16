// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/HUD/HUDComponents/ActionPointsBarWidget.h"
#include "CharacterProgression/GameStat.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/HUD/HUDComponents/ActionPointWidget.h"

#define LOCTEXT_NAMESPACE "UI"

void UActionPointsBarWidget::Bind(UGameStat* InAPStat)
{
	if (ensure(InAPStat))
	{
		APStat = InAPStat;
		MaxAPStat = APStat->GetMaxValueStat();

		OnAPChangeHandle = APStat->OnChange.AddUObject(this, &UActionPointsBarWidget::OnAPChange);
		OnMaxAPChangeHandle = MaxAPStat->OnChange.AddUObject(this, &UActionPointsBarWidget::OnMaxAPChange);

		OnAPChange(APStat->Get());
		OnMaxAPChange(MaxAPStat->Get());
	}
}

void UActionPointsBarWidget::RemoveBinding()
{
	APStat->OnChange.Remove(OnAPChangeHandle);
	MaxAPStat->OnChange.Remove(OnMaxAPChangeHandle);
	ActionPointsBar->ClearChildren();

	APStat = nullptr;
	MaxAPStat = nullptr;
}

void UActionPointsBarWidget::SetSpendingIndication(int32 APCount)
{
	bSpendingIndication = true;

	float IndicatingAPCost = FMath::Clamp(APCount, 0.f, APStat->Get());
	int32 SpentAP = MaxAPStat->Get() - APStat->Get();
	int32 LowerBound = ActionPointWidgets.Num() - IndicatingAPCost - SpentAP;

	for (int32 i = ActionPointWidgets.Num() - 1; i >= LowerBound; --i)
	{
		if (ActionPointWidgets[i]->GetIcon() == EAPIcon::HasPoint)
		{
			ActionPointWidgets[i]->SetIcon(EAPIcon::Spending);
		}
	}

	int32 UpperBound = ActionPointWidgets.Num() - IndicatingAPCost - SpentAP;

	for (int32 i = 0; i < UpperBound; ++i)
	{
		if (ActionPointWidgets[i]->GetIcon() != EAPIcon::Spent)
		{
			ActionPointWidgets[i]->SetIcon(EAPIcon::HasPoint);
		}
	}
}

void UActionPointsBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bSpendingIndication)
	{
		for (UActionPointWidget* APWidget : ActionPointWidgets)
		{
			if (APWidget->GetIcon() == EAPIcon::Spending)
			{
				APWidget->SetIcon(EAPIcon::HasPoint);
			}
		}
	}

	bSpendingIndication = false;
}

void UActionPointsBarWidget::OnMaxAPChange(float MaxAPValue)
{
	OnAPChange(APStat->Get());
}

void UActionPointsBarWidget::OnAPChange(float APValue)
{
	if (!ActionPointWidgetClass)
	{
		return;
	}

	if (MaxAPStat->Get() != 0.f) //-V550
	{
		int32 APValueInt = FMath::RoundToInt32(APValue);

		ActionPointsBar->ClearChildren();
		ActionPointWidgets.Empty();
		for (int32 i = 0; i < APValueInt; ++i)
		{
			auto APWidget = CreateWidget<UActionPointWidget>(this, *ActionPointWidgetClass);
			APWidget->SetIcon(EAPIcon::HasPoint);
			ActionPointsBar->AddChild(APWidget);
			ActionPointWidgets.Add(APWidget);
		}

		for (int32 i = APValueInt; i < FMath::RoundToInt32(MaxAPStat->Get()); ++i)
		{
			auto APWidget = CreateWidget<UActionPointWidget>(this, *ActionPointWidgetClass);
			APWidget->SetIcon(EAPIcon::Spent);
			ActionPointsBar->AddChild(APWidget);
			ActionPointWidgets.Add(APWidget);
		}
	}
}
#undef LOCTEXT_NAMESPACE