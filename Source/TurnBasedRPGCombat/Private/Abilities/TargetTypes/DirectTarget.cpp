// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/DirectTarget.h"
#include "Characters/RPGCharacter.h"
#include "Utility/TurnBasedUtility.h"

void UDirectTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	float DistanceToTarget = FVector::Distance(CursorHitResult.Location, ControlledCharacter->GetLocation());
	bEnableRotating = DistanceToTarget > MinDistanceForRotation;

	AActor* HitActor = CursorHitResult.GetActor();

	DisableOutline(Target.Actor);

	if (IsDetected(HitActor) || HitActor == ControlledCharacter)
	{
		Target.Actor = HitActor;

		if (IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(AbilityOwner))
		{
			ETeamAttitude::Type Attitude = OwnerTeamAgent->GetTeamAttitudeTowards(*HitActor);
			EOutlineColor OutlineColor = UTurnBasedUtility::AttitudeToOutlineColor(Attitude);
			EnableOutline(HitActor, OutlineColor);
		}
	}
}

void UDirectTarget::StopTargeting()
{
	DisableOutline(Target.Actor);
}
