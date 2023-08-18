// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "TurnBased/TurnBasedBattle.h"
#include "EngineUtils.h"
#include "ActorComponents/TurnBasedComponent.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"

DEFINE_LOG_CATEGORY(LogTurnBasedBattle);

ATurnBasedBattle::ATurnBasedBattle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATurnBasedBattle::FillTurnQueues(const TArray<TScriptInterface<ITBBattleParticipant>> QueuedParticipants)
{
	CurrentTurnQueue = QueuedParticipants;
	NextTurnQueue = QueuedParticipants;

	CurrentTurnQueue.Sort(FInitiativeLess());
	NextTurnQueue.Sort(FInitiativeLess());
}

void ATurnBasedBattle::ShuffleParticipants(TArray<TScriptInterface<ITBBattleParticipant>>& Participants)
{
	const int32 LastIndex = Participants.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		const int32 Index = FMath::RandRange(i, LastIndex);
		if (i != Index)
		{
			Participants.Swap(i, Index);
		}
	}
}

int32 Dice6Roll()
{
	return FMath::RandRange(1, 6);
}

bool FInitiativeLess::operator()(const TScriptInterface<ITBBattleParticipant>& Left,
								 const TScriptInterface<ITBBattleParticipant>& Right) const
{
	const int32 LeftInitiative = Left->Stats()->Get(SN_Initiative) + Dice6Roll();
	const int32 RightInitiative = Right->Stats()->Get(SN_Initiative) + Dice6Roll();
	return LeftInitiative < RightInitiative;
}

void ATurnBasedBattle::Start()
{
	if (BattleParticipants.IsEmpty())
	{
		UE_LOG(LogTurnBasedBattle, Error, TEXT("%s is trying to start with 0 participants"), *GetName())
		return;
	}

	FillTurnQueues(BattleParticipants);

	for (const FBattleParticipant BattleParticipant : BattleParticipants)
	{
		BattleParticipant->GetTurnBasedComponent()->EnterBattle(this);
	}

	NextParticipant();
}

void ATurnBasedBattle::End()
{
}

void ATurnBasedBattle::AddParticipant(const TScriptInterface<ITBBattleParticipant> Participant)
{
	if (BattleParticipants.Contains(Participant))
	{
		return;
	}

	BattleParticipants.Add(Participant);
}

void ATurnBasedBattle::AddParticipants(TArray<TScriptInterface<ITBBattleParticipant>> Participants)
{
	for (const FBattleParticipant Character : Participants)
	{
		AddParticipant(Character);
	}
}

void ATurnBasedBattle::AddNextTurnQueue()
{
	NextTurnQueue = BattleParticipants;
	NextTurnQueue.Sort(FInitiativeLess());
}

void ATurnBasedBattle::NextParticipant()
{
	if (CurrentTurnQueue.IsEmpty())
	{
		SetCurrentTurnQueue(NextTurnQueue);
		AddNextTurnQueue();
		OnAddNextTurnQueue.Broadcast(CurrentTurnQueue, NextTurnQueue);
	}

	const FBattleParticipant NextParticipant = CurrentTurnQueue.Pop();
	UTurnBasedComponent* TurnBasedComponent = NextParticipant->GetTurnBasedComponent();
	TurnBasedComponent->StartTurn();
	SetCurrentParticipant(NextParticipant);

	OnSetNextParticipant.Broadcast(NextParticipant);
}

TArray<TScriptInterface<ITBBattleParticipant>> ATurnBasedBattle::GetParticipantsWithAttitudeTo(AActor* AttitudeCheckActor,
                                                                                               const ETeamAttitude::Type Attitude) const
{
	TArray<TScriptInterface<ITBBattleParticipant>> Result;
	if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(AttitudeCheckActor))
	{
		for (FBattleParticipant BattleParticipant : BattleParticipants)
		{
			if (TeamAgent->GetTeamAttitudeTowards(*BattleParticipant->GetActor()) == Attitude)
			{
				Result.Add(BattleParticipant);
			}
		}
	}

	return Result;
}

void ATurnBasedBattle::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARPGCharacter> CharacterIterator(GetWorld()); CharacterIterator; ++CharacterIterator)
	{
		AddParticipant(*CharacterIterator);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATurnBasedBattle::Start, 0.2f);
}
