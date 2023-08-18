// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/AnimNotifies/SpawnProjectileNotify.h"

#include "Characters/RPGCharacter.h"

void USpawnProjectileNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (const auto OwnerCharacter = Cast<ARPGCharacter>(MeshComp->GetOwner()))
	{
		OwnerCharacter->OnProjectileSpawn.ExecuteIfBound();
	}
}
