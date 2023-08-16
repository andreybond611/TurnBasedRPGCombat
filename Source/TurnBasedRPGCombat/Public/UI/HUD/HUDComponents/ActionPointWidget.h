// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionPointWidget.generated.h"

class UImage;

UENUM(BlueprintType)
enum class EAPIcon : uint8
{
	HasPoint,
	Spending,
	Spent
};

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UActionPointWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetIcon(EAPIcon InIcon);
	EAPIcon GetIcon() const { return Icon; }

protected:
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY(EditAnywhere)
	EAPIcon Icon;
	UPROPERTY(EditAnywhere)
	UTexture2D* HasPointTexture;
	UPROPERTY(EditAnywhere)
	UTexture2D* SpendingTexture;
	UPROPERTY(EditAnywhere)
	UTexture2D* SpentTexture;

	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	UTexture2D* GetTextureFromAPIcon(EAPIcon InIcon) const;
};
