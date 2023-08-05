// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPGPlayerState.generated.h"

class ARPGCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterAddedToParty, ARPGCharacter*, AddedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterRemovedFromParty, ARPGCharacter*, RemovedCharacter);

/**
 * Manages player party
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API ARPGPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	TArray<ARPGCharacter*> GetPlayerParty()const { return PlayerParty; }

	void AddCharacterToParty(ARPGCharacter* InCharacter);
	void RemoveCharacterFromParty(ARPGCharacter* InCharacter);

	UPROPERTY(BlueprintAssignable)
	FOnCharacterAddedToParty OnCharacterAddedToParty;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterRemovedFromParty OnCharacterRemovedFromParty;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<ARPGCharacter*> PlayerParty;

	/** If true adds characters to the player's party from level with bPlayableAtStart property being true */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Party", meta = (AllowPrivateAccess = "true"))
	bool bGrabCharactersFromLevel = true;

	/** Teams that the characters were in, before they were added to the party */
	TMap<ARPGCharacter*, uint8> OriginalTeams;

	void AddPlayableCharactersToParty();
};
