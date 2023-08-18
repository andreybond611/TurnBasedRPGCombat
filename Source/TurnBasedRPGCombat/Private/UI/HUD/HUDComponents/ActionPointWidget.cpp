// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "UI/HUD/HUDComponents/ActionPointWidget.h"
#include "Components/Image.h"

void UActionPointWidget::SetIcon(const EAPIcon InIcon)
{
	UTexture2D* Texture = GetTextureFromAPIcon(InIcon);
	IconImage->SetBrushFromTexture(Texture);
	Icon = InIcon;
}

void UActionPointWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
}

UTexture2D* UActionPointWidget::GetTextureFromAPIcon(const EAPIcon InIcon) const
{
	switch (InIcon)
	{
	case EAPIcon::HasPoint: return HasPointTexture;
	case EAPIcon::Spending: return SpendingTexture;
	case EAPIcon::Spent: return SpentTexture;
	default: checkNoEntry() return nullptr;
	}
}
