// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "UI/FloatingNumberWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/TextBlock.h"

void UFloatingNumberWidget::Init(const int32 Number, const FColor Color)
{
	NumberText->SetText(FText::AsNumber(Number));
	NumberText->SetColorAndOpacity(Color);

	UUMGSequencePlayer* SequencePlayer = PlayAnimation(FloatUp);
	SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&) { RemoveFromParent(); });
}

void UFloatingNumberWidget::Init(const FText& Message, const FColor Color)
{
	NumberText->SetText(Message);
	NumberText->SetColorAndOpacity(Color);

	UUMGSequencePlayer* SequencePlayer = PlayAnimation(FloatUp);
	SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&) { RemoveFromParent(); });
}
