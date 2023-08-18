// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/AnimNotifies/WeaponHitNotify.h"

#include "Characters/RPGCharacter.h"

void UWeaponHitNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (const auto Character = Cast<ARPGCharacter>(MeshComp->GetOwner()))
	{
		Character->OnHitLanded.ExecuteIfBound();
	}
}
