// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/AnimNotifies/AbilityCallbackNotify.h"

#include "Abilities/Ability.h"
#include "Abilities/AbilityComponent.h"
#include "Characters/RPGCharacter.h"

void UAbilityCallbackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (const auto* Character = Cast<ARPGCharacter>(MeshComp->GetOwner()))
	{
		const UAbility* ExecutingAbility = Character->GetAbilityComponent()->FindAbility(CallbackAbility);
		if (ensure(ExecutingAbility))
		{
			ExecutingAbility->OnAnimNotifyExecuted.Broadcast();
		}
	}
}
