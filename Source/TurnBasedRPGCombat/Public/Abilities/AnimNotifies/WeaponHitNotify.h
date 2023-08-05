// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WeaponHitNotify.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UWeaponHitNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
