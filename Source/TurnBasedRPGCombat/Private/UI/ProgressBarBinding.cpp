// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "UI/ProgressBarBinding.h"
#include "CharacterProgression/GameStat.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UProgressBarBinding::Bind(UGameStat* InStat, UProgressBar* InProgressBar, UTextBlock* InTextBlock)
{
	if (ensure(InStat))
	{
		check(InProgressBar && InTextBlock);

		CurrentStat = InStat;
		MaxStat = CurrentStat->GetMaxValueStat();
		ProgressBar = InProgressBar;
		TextBlock = InTextBlock;

		if (ensureMsgf(MaxStat, TEXT("ProgressBarBinding tries to bind a stat with no bound MaxStat")))
		{
			CurrentStatHandle = CurrentStat->OnChange.AddUObject(this, &UProgressBarBinding::OnStatChanged);
			MaxStatHandle = MaxStat->OnChange.AddUObject(this, &UProgressBarBinding::OnMaxStatChanged);

			OnStatChanged(CurrentStat->Get());
			OnMaxStatChanged(MaxStat->Get());
		}
	}
}

void UProgressBarBinding::Unbind()
{
	if (CurrentStat && MaxStat)
	{
		CurrentStat->OnChange.Remove(CurrentStatHandle);
		MaxStat->OnChange.Remove(MaxStatHandle);

		OnMaxStatChanged(0.f);
	}

	CurrentStat = nullptr;
	MaxStat = nullptr;
	ProgressBar = nullptr;
	TextBlock = nullptr;
}

void UProgressBarBinding::OnStatChanged(float Value)
{
	ProgressBar->SetPercent(GetStatPercent(Value, MaxStat->Get()));
	SetText(Value, MaxStat->Get());
}

void UProgressBarBinding::SetText(float CurrentHealth, float MaxHealth)
{
	FText Text = FText::Format(NSLOCTEXT("UI", "HealthBarNumbers", "{0}/{1}"), static_cast<int32>(CurrentHealth), static_cast<int32>(MaxHealth));
	TextBlock->SetText(Text);
}

void UProgressBarBinding::OnMaxStatChanged(float Value)
{
	ProgressBar->SetPercent(GetStatPercent(CurrentStat->Get(), Value));
	SetText(CurrentStat->Get(), Value);
}

float UProgressBarBinding::GetStatPercent(float InCurrentStat, float InMaxStat) const
{
	return InMaxStat == 0.f ? 0.f : InCurrentStat / InMaxStat;
}