// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/DirectTarget.h"
#include "Characters/RPGCharacter.h"
#include "Utility/TurnBasedUtility.h"

void UDirectTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	const float DistanceToTarget = FVector::Distance(CursorHitResult.Location, ControlledCharacter->GetLocation());
	bEnableRotating = DistanceToTarget > MinDistanceForRotation;

	AActor* HitActor = CursorHitResult.GetActor();
	bAllowedExecute = false;

	DisableOutline(Target.Actor);

	if (IsDetected(HitActor) || HitActor == ControlledCharacter)
	{
		Target.Actor = HitActor;
		bAllowedExecute = true;

		if (const IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(AbilityOwner))
		{
			const ETeamAttitude::Type Attitude = OwnerTeamAgent->GetTeamAttitudeTowards(*HitActor);
			const EOutlineColor OutlineColor = UTurnBasedUtility::AttitudeToOutlineColor(Attitude);
			EnableOutline(HitActor, OutlineColor);
		}
	}
}

void UDirectTarget::StopTargeting()
{
	DisableOutline(Target.Actor);
}
