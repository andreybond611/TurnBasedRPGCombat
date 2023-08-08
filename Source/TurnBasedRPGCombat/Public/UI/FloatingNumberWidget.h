// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingNumberWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UFloatingNumberWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(int32 Number, FColor Color = FColor::White);
	void Init(const FText& Message, FColor Color = FColor::White);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumberText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FloatUp;
};
