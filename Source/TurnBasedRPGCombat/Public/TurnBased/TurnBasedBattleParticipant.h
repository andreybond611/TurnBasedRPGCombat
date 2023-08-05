// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurnBasedBattleParticipant.generated.h"

class IGenericTeamAgentInterface;
class UStatsComponent;
class UTurnBasedComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTBBattleParticipant : public UInterface
{
	GENERATED_BODY()
};

/**
 * Actor that can be in a turn based battle
 */
class TURNBASEDRPGCOMBAT_API ITBBattleParticipant
{
	GENERATED_BODY()

public:
	virtual UTurnBasedComponent* GetTurnBasedComponent() const;
	virtual UStatsComponent* Stats() const;
	virtual UTexture2D* GetPortrait() const;
	FVector GetLocation();
	AActor* GetActor();
	IGenericTeamAgentInterface* GetGenericTeamAgentInterface();
	virtual bool IsPlayerControlled() const;
};
