// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "UI/FloatingDamageNumberWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/TextBlock.h"

void UFloatingDamageNumberWidget::Init(int32 Damage, FColor Color)
{
	DamageText->SetText(FText::AsNumber(Damage));
	DamageText->SetColorAndOpacity(Color);

	UUMGSequencePlayer* SequencePlayer = PlayAnimation(FloatUp);
	SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&) { RemoveFromParent(); });
}
