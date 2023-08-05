// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurnBasedAIController.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API ATurnBasedAIController : public AAIController
{
	GENERATED_BODY()
public:
	void SetPlayerControlled(bool bPlayerControlled);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* Behavior;

	UFUNCTION()
	void StartTurn();
	UFUNCTION()
	void EndTurn();

	UFUNCTION()
	void EnterBattle();
	UFUNCTION()
	void ExitBattle();
};
