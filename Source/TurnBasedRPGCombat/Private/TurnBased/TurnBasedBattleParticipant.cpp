// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "TurnBased/TurnBasedBattleParticipant.h"

#include "GenericTeamAgentInterface.h"

// Add default functionality here for any ITurnBasedBattleParticipant functions that are not pure virtual.
UTurnBasedComponent* ITBBattleParticipant::GetTurnBasedComponent() const
{
	unimplemented()
	return nullptr;
}

UStatsComponent* ITBBattleParticipant::Stats() const
{
	unimplemented()
	return nullptr;
}

UTexture2D* ITBBattleParticipant::GetPortrait() const
{
	unimplemented()
	return nullptr;
}

FVector ITBBattleParticipant::GetLocation()
{
	if (auto Actor = GetActor())
	{
		return Actor->GetActorLocation();
	}
	return FVector::ZeroVector;
}

AActor* ITBBattleParticipant::GetActor()
{
	return Cast<AActor>(this);
}

IGenericTeamAgentInterface* ITBBattleParticipant::GetGenericTeamAgentInterface()
{
	return Cast<IGenericTeamAgentInterface>(GetActor());
}

bool ITBBattleParticipant::IsPlayerControlled() const
{
	return false;
}
