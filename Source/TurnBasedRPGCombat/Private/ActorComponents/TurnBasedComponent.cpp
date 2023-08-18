// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "ActorComponents/TurnBasedComponent.h"
#include "TurnBased/TurnBasedBattle.h"

UTurnBasedComponent::UTurnBasedComponent()
{
}

void UTurnBasedComponent::StartTurn()
{
	bCurrentTurn = true;

	OnTurnBegin.Broadcast();
}

void UTurnBasedComponent::EndTurn()
{
	bCurrentTurn = false;
	OnTurnEnd.Broadcast();

	CurrentBattle ? CurrentBattle->NextParticipant() : EndBattle();
}

void UTurnBasedComponent::EnterBattle(ATurnBasedBattle* Battle)
{
	CurrentBattle = Battle;

	OnEnterBattle.Broadcast();
}

void UTurnBasedComponent::EndBattle()
{
	CurrentBattle = nullptr;

	OnExitBattle.Broadcast();
}

ATurnBasedBattle* UTurnBasedComponent::GetCurrentBattle()
{
	return CurrentBattle;
}

void UTurnBasedComponent::BeginPlay()
{
	Super::BeginPlay();
}
