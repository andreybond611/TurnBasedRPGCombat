// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetInfoWidget.generated.h"

struct FPriorityText;
struct FTargetInfo;
class UVerticalBox;
/**
 * Shows info about a target which is under a mouse cursor
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UTargetInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(const TArray<FPriorityText>& InInfoSections);

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ContentPanel;
};
