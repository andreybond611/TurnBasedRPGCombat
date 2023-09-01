// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/MultiTarget.h"
#include "MultiAreaTarget.generated.h"

class AAOESphereActor;

/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UMultiAreaTarget : public UMultiTarget
{
	GENERATED_BODY()
public:
	virtual void ConfirmTarget() override;
	virtual void CancelConfirmedTarget() override;

	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;
	virtual void StopTargeting() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAOESphereActor> ConfirmedTargetClass;

	UPROPERTY()
	TArray<AActor*> ConfirmedTargetActors;
};
