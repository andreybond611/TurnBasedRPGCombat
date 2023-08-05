// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/Effects/Effect.h"

void UEffect::ReduceTurn()
{
	--TurnCount;
}

void UEffect::ApplyTo(AActor* InActor)
{

}

bool UEffect::CanApplyTo(AActor* InActor)
{
	return true;
}

void UEffect::Remove()
{
	
}

bool UEffect::CanRemove()
{
	return true;
}

void UEffect::Stack(UEffect* Effect)
{
	if (TurnCount < Effect->GetTurnCount())
	{
		TurnCount = Effect->GetTurnCount();
	}
}

void UEffect::ApplyVisuals()
{

}

void UEffect::RemoveVisuals()
{

}

void UEffect::OnOwnerStartTurn()
{

}
