// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utility/TurnBasedTypes.h"
#include "FloatingNumbersWidget.generated.h"

class UFloatingNumberWidget;
class UCanvasPanel;
/**
 * Shows damage numbers around a recently damaged actor
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UFloatingNumbersWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void AddFloatingNumber(int32 Number, FColor Color = FColor::White);
	void AddFloatingNumber(int32 Number, EFloatingNumberColor Color);

	FColor GetColorFromDamageType(EFloatingNumberColor DamageType) const;
	void AddFloatingMessage(const FText& Message, FColor Color = FColor::White);

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* FloatingNumbersPanel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFloatingNumberWidget> FloatingDamageWidgetClass;

	UPROPERTY(EditAnywhere)
	FColor PhysicalDamageColor;
	UPROPERTY(EditAnywhere)
	FColor FireDamageColor;
	UPROPERTY(EditAnywhere)
	FColor EarthDamageColor;
	UPROPERTY(EditAnywhere)
	FColor PoisonDamageColor;
	UPROPERTY(EditAnywhere)
	FColor IceDamageColor;

	UPROPERTY(EditAnywhere)
	FColor MagicArmorDamageColor;
	UPROPERTY(EditAnywhere)
	FColor PhysicalArmorDamageColor;

	UPROPERTY(EditAnywhere)
	FColor HealColor;
};
