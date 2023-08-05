// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/HUD/HUDComponents/ActionPointsBarWidget.h"
#include "CharacterProgression/GameStat.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "UI"

void UActionPointsBarWidget::Bind(UGameStat* InAPStat, UGameStat* InMaxAPStat)
{
	APStat = InAPStat;
	MaxAPStat = InMaxAPStat;

	OnAPChangeHandle = APStat->OnChange.AddUObject(this, &UActionPointsBarWidget::OnAPChange);
	OnMaxAPChangeHandle = MaxAPStat->OnChange.AddUObject(this, &UActionPointsBarWidget::OnMaxAPChange);

	OnAPChange(APStat->Get());
	OnMaxAPChange(MaxAPStat->Get());
}

void UActionPointsBarWidget::RemoveBinding()
{
	APStat->OnChange.Remove(OnAPChangeHandle);
	MaxAPStat->OnChange.Remove(OnMaxAPChangeHandle);
	ActionPointsBar->SetPercent(0.f);

	APStat = nullptr;
	MaxAPStat = nullptr;
}

void UActionPointsBarWidget::OnMaxAPChange(float MaxAPValue)
{
	OnAPChange(APStat->Get());
}

void UActionPointsBarWidget::OnAPChange(float APValue)
{
	if (MaxAPStat->Get() != 0.f)
	{
		float Percent = APValue / MaxAPStat->Get();
		ActionPointsBar->SetPercent(Percent);

		FText Text =
			FText::Format(LOCTEXT("ActionPointsBar", "ActionPoints: {0} / {1}"), FMath::RoundToInt32(APValue), FMath::RoundToInt32(MaxAPStat->Get()));

		APText->SetText(Text);
	}
}

#undef LOCTEXT_NAMESPACE