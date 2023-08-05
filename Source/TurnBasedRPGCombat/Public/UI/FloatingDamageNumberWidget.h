// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingDamageNumberWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UFloatingDamageNumberWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(int32 Damage, FColor Color = FColor::White);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FloatUp;
};
