// Copyright 2022 Andrei Bondarenko. All rights reserved


#include "Interaction/Interactable.h"

void IInteractable::OnMouseOverEnter()
{
	unimplemented();
}

void IInteractable::OnMouseOverLeave()
{
	unimplemented();
}

int32 IInteractable::GetInteractionCost()
{
	unimplemented();
	return INDEX_NONE;
}

bool IInteractable::CanInteractWith(ARPGCharacter* InteractingCharacter)
{
	unimplemented();
	return false;
}

void IInteractable::InteractWith(ARPGCharacter* InteractingCharacter)
{
	unimplemented();
}
