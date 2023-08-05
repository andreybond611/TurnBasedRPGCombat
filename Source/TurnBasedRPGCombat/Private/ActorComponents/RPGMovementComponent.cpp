// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "ActorComponents/RPGMovementComponent.h"

#include "CharacterProgression/GameStat.h"
#include "Characters/Animations/RPGAnimInstance.h"
#include "GameFramework/Character.h"

URPGMovementComponent::URPGMovementComponent()
{
}

void URPGMovementComponent::PostLoad()
{
	Super::PostLoad();

	WalkSpeedStat = UGameStat::Create(MaxWalkSpeed, NSLOCTEXT("Stats", "WalkSpeed Stat", "WalkSpeed"), 0.f, TNumericLimits<float>::Max());
	if (ensure(WalkSpeedStat))
	{
		WalkSpeedStat->OnChange.AddUObject(this, &URPGMovementComponent::OnMoveSpeedStatChanged);
	}
}

void URPGMovementComponent::OnMoveSpeedStatChanged(float MoveSpeed)
{
	MaxWalkSpeed = MoveSpeed;

	if (auto Character = Cast<ACharacter>(GetOwner()))
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (auto RPGAnimInstance = Cast<URPGAnimInstance>(AnimInstance))
		{
			RPGAnimInstance->SetMovementAnimRate(WalkSpeedStat->GetMultiplier());
		}
	}
}

