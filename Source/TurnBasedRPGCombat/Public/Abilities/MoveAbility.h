// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "Abilities/SimpleAbility.h"
#include "Navigation/PathFollowingComponent.h"
#include "MoveAbility.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveToActorSucceeded, AActor*);

/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UMoveAbility : public USimpleAbility
{
	GENERATED_BODY()
public:
	virtual void AddToCharacter(ARPGCharacter* Character) override;
	virtual void StartAbility() override;
	virtual void RemoveFromCharacter(ARPGCharacter* Character) override;

	FOnMoveToActorSucceeded OnMoveToActorSucceeded;


	void SetAcceptanceRadius(float InAcceptanceRadius) { AcceptanceRadius = InAcceptanceRadius; }
private:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 120.f;
	FDelegateHandle RequestFinishedDelegateHandle;

	UPROPERTY()
	UPathFollowingComponent* PathFollowingComponent;
	void MoveFinished(FAIRequestID AIRequestID, const FPathFollowingResult& PathFollowingResult);
};
