// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class ARPGCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURNBASEDRPGCOMBAT_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnMouseOverEnter();
	virtual void OnMouseOverLeave();

	virtual int32 GetInteractionCost();
	virtual bool CanInteractWith(ARPGCharacter* InteractingCharacter);

	virtual void InteractWith(ARPGCharacter* InteractingCharacter);
};
