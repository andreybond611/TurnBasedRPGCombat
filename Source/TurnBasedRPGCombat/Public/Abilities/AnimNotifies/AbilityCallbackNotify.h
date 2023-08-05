// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AbilityCallbackNotify.generated.h"

class UAbility;

/**
 * Calls delegate on CallbackAbility
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UAbilityCallbackNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAbility> CallbackAbility;
};
