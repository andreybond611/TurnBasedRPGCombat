// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBarBinding;
class UTextBlock;
class UProgressBar;
class UGameStat;
class UStatsComponent;
/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Bind(UStatsComponent* StatsToBind);
	void Unbind();
	bool IsBound() const;

	void SetShowHealthNumbersOnlyOnMouseOver(bool bShow) { bShowHealthNumberOnlyOnMouseOver = bShow; }

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* PhysicalArmorProgressBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PhysicalArmorText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MagicArmorProgressBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MagicArmorText;

	UPROPERTY(EditAnywhere)
	UProgressBarBinding* HealthBinding;
	UPROPERTY(EditAnywhere)
	UProgressBarBinding* MagicArmorBinding;
	UPROPERTY(EditAnywhere)
	UProgressBarBinding* PhysicalArmorBinding;

	UPROPERTY(EditAnywhere)
	bool bShowHealthNumberOnlyOnMouseOver;

};
