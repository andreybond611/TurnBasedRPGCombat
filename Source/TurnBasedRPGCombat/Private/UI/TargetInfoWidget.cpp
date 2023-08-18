// Copyright 2022 Andrei Bondarenko. All rights reserved


#include "UI/TargetInfoWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/UITypes.h"

#define LOCTEXT_NAMESPACE "UI"

void UTargetInfoWidget::Init(const TArray<FPriorityText>& InInfoSections)
{
	TArray<FPriorityText> InfoSections = InInfoSections;

	while (!InfoSections.IsEmpty())
	{
		FPriorityText Text;
		InfoSections.HeapPop(Text, PriorityTextPredicate());
		const auto TextBlock = WidgetTree->ConstructWidget<UTextBlock>();

		TextBlock->SetText(Text.Text);
		TextBlock->SetColorAndOpacity(Text.Color);
		ContentPanel->AddChildToVerticalBox(TextBlock);
	}
}

#undef LOCTEXT_NAMESPACE
