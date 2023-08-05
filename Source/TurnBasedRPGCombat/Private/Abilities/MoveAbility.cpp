// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/MoveAbility.h"
#include "Characters/RPGCharacter.h"

void UMoveAbility::AddToCharacter(ARPGCharacter* Character)
{
	Super::AddToCharacter(Character);

	PathFollowingComponent = Owner->FindPathFollowingComponent();
	check(PathFollowingComponent)
}

void UMoveAbility::StartAbility()
{
	if (GetTarget().Actor)
	{
		RequestFinishedDelegateHandle = PathFollowingComponent->OnRequestFinished.AddUObject(this, &UMoveAbility::MoveFinished);

		Owner->MoveToActor(GetTarget().Actor, AcceptanceRadius);
	}
	else
	{
		PathFollowingComponent->OnRequestFinished.Remove(RequestFinishedDelegateHandle);
		Owner->MoveToLocation(GetTarget().Location);
	}
}

void UMoveAbility::RemoveFromCharacter(ARPGCharacter* Character)
{
	Super::RemoveFromCharacter(Character);

	PathFollowingComponent->OnRequestFinished.Remove(RequestFinishedDelegateHandle);
}

void UMoveAbility::MoveFinished(FAIRequestID AIRequestID, const FPathFollowingResult& PathFollowingResult)
{
	if (PathFollowingResult.IsSuccess())
	{
		OnMoveToActorSucceeded.Broadcast(GetTarget().Actor);
	}
}
