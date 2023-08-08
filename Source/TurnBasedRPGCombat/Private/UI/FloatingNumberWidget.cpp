// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "UI/FloatingNumberWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/TextBlock.h"

void UFloatingNumberWidget::Init(int32 Number, FColor Color)
{
	NumberText->SetText(FText::AsNumber(Number));
	NumberText->SetColorAndOpacity(Color);

	UUMGSequencePlayer* SequencePlayer = PlayAnimation(FloatUp);
	SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&) { RemoveFromParent(); });
}

void UFloatingNumberWidget::Init(const FText& Message, FColor Color)
{
	NumberText->SetText(Message);
	NumberText->SetColorAndOpacity(Color);

	UUMGSequencePlayer* SequencePlayer = PlayAnimation(FloatUp);
	SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&) { RemoveFromParent(); });
}
