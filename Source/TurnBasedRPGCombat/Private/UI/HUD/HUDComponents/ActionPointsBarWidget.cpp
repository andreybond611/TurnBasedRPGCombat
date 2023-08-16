// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/HUD/HUDComponents/ActionPointsBarWidget.h"
#include "CharacterProgression/GameStat.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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
	// ActionPointsBar->SetPercent(0.f);

	APStat = nullptr;
	MaxAPStat = nullptr;
}

void UActionPointsBarWidget::OnMaxAPChange(float MaxAPValue)
{
	OnAPChange(APStat->Get());
}

void UActionPointsBarWidget::OnAPChange(float APValue)
{
	if (MaxAPStat->Get() != 0.f) //-V550
	{

		float Percent = APValue / MaxAPStat->Get();
		// ActionPointsBar->SetPercent(Percent);
	}
}

#undef LOCTEXT_NAMESPACE