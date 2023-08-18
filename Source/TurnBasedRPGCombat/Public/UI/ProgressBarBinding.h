// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "ProgressBarBinding.generated.h"

class UGameStat;
class UTextBlock;
class UProgressBar;
/**
 * Binds UGameStat to a progress bar
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UProgressBarBinding : public UObject
{
	GENERATED_BODY()
public:
	void Bind(UGameStat* InStat, UProgressBar* InProgressBar, UTextBlock* InTextBlock);
	void Unbind();

	bool IsBound() const { return CurrentStat != nullptr; }

private:
	UPROPERTY()
	UGameStat* CurrentStat;
	UPROPERTY()
	UGameStat* MaxStat;

	FDelegateHandle CurrentStatHandle;
	FDelegateHandle MaxStatHandle;

	UPROPERTY()
	UProgressBar* ProgressBar;
	UPROPERTY()
	UTextBlock* TextBlock;

	UFUNCTION()
	void OnStatChanged(float Value);


	UFUNCTION()
	void OnMaxStatChanged(float Value);

	float GetStatPercent(float InCurrentStat, float InMaxStat) const;
	void SetText(float CurrentHealth, float MaxHealth);
};
