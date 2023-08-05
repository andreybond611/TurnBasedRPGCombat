// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnBasedComponent.generated.h"

class ATurnBasedBattle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterBattle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitBattle);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TURNBASEDRPGCOMBAT_API UTurnBasedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTurnBasedComponent();

	UFUNCTION(BlueprintCallable)
	void StartTurn();

	UFUNCTION(BlueprintCallable)
	void EndTurn();

	UFUNCTION(BlueprintPure)
	bool IsCurrentTurn()const { return bCurrentTurn; }

	void EnterBattle(ATurnBasedBattle* Battle);

	void EndBattle();

	UFUNCTION(BlueprintPure)
	ATurnBasedBattle* GetCurrentBattle();

	UPROPERTY(BlueprintAssignable)
	FOnTurnBegin OnTurnBegin;

	UPROPERTY(BlueprintAssignable)
	FOnTurnEnd OnTurnEnd;

	UPROPERTY(BlueprintAssignable)
	FOnEnterBattle OnEnterBattle;

	UPROPERTY(BlueprintAssignable)
	FOnExitBattle OnExitBattle;

	UPROPERTY(EditAnywhere)
	float EndTurnCooldown = 1.f;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleInstanceOnly)
	bool bCurrentTurn = false;

	UPROPERTY(Transient)
	ATurnBasedBattle* CurrentBattle;
};
