// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Actor.h"
#include "TurnBasedBattle.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTurnBasedBattle, Log, All);

class ITurnBasedBattleParticipant;
class ARPGCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetNextParticipant, TScriptInterface<ITBBattleParticipant>, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddNextTurnQueue,const TArray<TScriptInterface<ITBBattleParticipant>>&, CurrentTurnQueue,
											 const TArray<TScriptInterface<ITBBattleParticipant>>&, NextTurnQueue);

/**
 * Actor representation of a turn based battle, every battle should spawn one.
 * Manages beginning and ending of a battle, who's turn next and adding new combatants
 */
UCLASS(BlueprintType, Blueprintable)
class TURNBASEDRPGCOMBAT_API ATurnBasedBattle : public AActor
{
	GENERATED_BODY()

public:
	ATurnBasedBattle();

	using FBattleParticipant = TScriptInterface<ITBBattleParticipant>;

	UFUNCTION(BlueprintCallable)
	void FillTurnQueues(TArray<TScriptInterface<ITBBattleParticipant>> QueuedParticipants);

	UFUNCTION(BlueprintCallable)
	void Start();

	UFUNCTION(BlueprintCallable)
	void End();

	UFUNCTION(BlueprintCallable)
	void AddParticipant(TScriptInterface<ITBBattleParticipant> Participant);

	UFUNCTION(BlueprintCallable)
	void AddParticipants(TArray<TScriptInterface<ITBBattleParticipant>> Participants);

	UFUNCTION(BlueprintCallable)
	void NextParticipant();

	UPROPERTY(BlueprintAssignable)
	FOnSetNextParticipant OnSetNextParticipant;
	UPROPERTY(BlueprintAssignable)
	FOnAddNextTurnQueue OnAddNextTurnQueue;

	void SetCurrentParticipant(FBattleParticipant Participant) { CurrentParticipant = Participant; }
	void SetCurrentTurnQueue(TArray<FBattleParticipant> TurnQueue) { CurrentTurnQueue = TurnQueue; }

	TArray<FBattleParticipant> GetParticipants() const { return BattleParticipants; }
	TArray<FBattleParticipant> GetParticipantsWithAttitudeTo(AActor* AttitudeCheckActor, ETeamAttitude::Type Attitude) const;
	TArray<FBattleParticipant> GetCurrentTurnQueue() const { return CurrentTurnQueue; }
	TArray<FBattleParticipant> GetNextTurnQueue() const { return NextTurnQueue; }
	FBattleParticipant GetCurrentCharacter() const { return CurrentParticipant; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TScriptInterface<ITBBattleParticipant>> BattleParticipants;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TScriptInterface<ITBBattleParticipant>> CurrentTurnQueue;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TScriptInterface<ITBBattleParticipant>> NextTurnQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 1000.f;

	UPROPERTY(VisibleInstanceOnly)
	TScriptInterface<ITBBattleParticipant> CurrentParticipant;

	void ShuffleParticipants(TArray<FBattleParticipant>& Participants);
	void AddNextTurnQueue();
};

int32 Dice6Roll();

struct FInitiativeLess
{
	bool operator()(const TScriptInterface<ITBBattleParticipant>& Left,
					const TScriptInterface<ITBBattleParticipant>& Right) const;
};
