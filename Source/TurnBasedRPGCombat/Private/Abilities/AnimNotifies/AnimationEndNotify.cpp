// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/AnimNotifies/AnimationEndNotify.h"

#include "Abilities/AbilityComponent.h"
#include "Abilities/ReadiableAbility.h"
#include "Characters/RPGCharacter.h"

void UAnimationEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (auto* Character = Cast<ARPGCharacter>(MeshComp->GetOwner()))
	{
		UAbility* ExecutingAbility = Character->GetAbilityComponent()->GetAbilityAt(AbilityIndex);
		ExecutingAbility->OnAnimationEnd.Broadcast();
	}
}
