// Copyright 2022 Andrei Bondarenko. All rights reserved


#include "UnrealFramework/RPGPlayerState.h"
#include "GenericTeamAgentInterface.h"
#include "EngineUtils.h"
#include "Characters/RPGCharacter.h"

void ARPGPlayerState::AddCharacterToParty(ARPGCharacter* InCharacter)
{
	if (InCharacter && !PlayerParty.Contains(InCharacter))
	{
		InCharacter->GiveControlToPlayer();
		OriginalTeams.Add(InCharacter, InCharacter->GetGenericTeamId().GetId());
		InCharacter->SetGenericTeamId(static_cast<uint8>(ERPGTeam::Player));

		PlayerParty.Add(InCharacter);
		OnCharacterAddedToParty.Broadcast(InCharacter);
	}
}

void ARPGPlayerState::RemoveCharacterFromParty(ARPGCharacter* InCharacter)
{
	if (InCharacter && PlayerParty.Contains(InCharacter))
	{
		InCharacter->RemoveControlFromPlayer();
		FGenericTeamId FoundTeamId = OriginalTeams.FindAndRemoveChecked(InCharacter);
		InCharacter->SetGenericTeamId(FoundTeamId);

		PlayerParty.Remove(InCharacter);
		OnCharacterRemovedFromParty.Broadcast(InCharacter);
	}
}

void ARPGPlayerState::AddPlayableCharactersToParty()
{
	if (bGrabCharactersFromLevel)
	{
		for (TActorIterator<ARPGCharacter> CharacterIterator(GetWorld()); CharacterIterator; ++CharacterIterator)
		{
			if ((*CharacterIterator)->GetIsPlayableAtStart())
			{
				AddCharacterToParty(*CharacterIterator);
			}
		}
	}
}

void ARPGPlayerState::BeginPlay()
{
	Super::BeginPlay();

	AddPlayableCharactersToParty();
}
