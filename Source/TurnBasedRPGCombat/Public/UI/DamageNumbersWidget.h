// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumbersWidget.generated.h"

class UFloatingDamageNumberWidget;
class UCanvasPanel;
/**
 * Shows damage numbers around a recently damaged actor
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UDamageNumbersWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void AddDamageNumber(int32 Damage, FColor Color = FColor::White);
	void AddDamageNumber(int32 Damage, EDamageColor DamageType);

	FColor GetColorFromDamageType(EDamageColor DamageType) const;

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* DamagePanel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFloatingDamageNumberWidget> FloatingDamageWidgetClass;

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
};
