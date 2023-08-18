// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Characters/Animations/RPGAnimInstance.h"

void URPGAnimInstance::SetAbilityAnimations(const FAbilityStateAnimations& AbilityAnimations)
{
	AbilityStart = AbilityAnimations.AbilityStart;
	AbilityReady = AbilityAnimations.AbilityReady;
	AbilityCancel = AbilityAnimations.AbilityCancel;
}

void URPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OnGameplayTagsChanged.AddUniqueDynamic(this, &URPGAnimInstance::GameplayTagChanged);
}

void URPGAnimInstance::SetMovementAnimRate(const float Rate)
{
	MovementAnimRate = Rate;
}

void URPGAnimInstance::GameplayTagChanged(const FGameplayTagContainer& GameplayTags)
{
	FGameplayTag StunTag;
	StunTag.FromExportString("Effect.Debuff.Stun");
	bStunt = GameplayTags.HasTag(StunTag);

}
