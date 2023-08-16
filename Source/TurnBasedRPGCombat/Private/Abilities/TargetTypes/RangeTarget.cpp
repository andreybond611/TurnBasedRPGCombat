// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/TargetTypes/RangeTarget.h"

#include "NavigationPath.h"
#include "Abilities/AbilityComponent.h"
#include "Abilities/MoveAbility.h"
#include "Abilities/TargetTypes/ArcTarget.h"
#include "Abilities/TargetTypes/MoveTarget.h"
#include "Characters/RPGCharacter.h"
#include "UnrealFramework/RPGPlayerController.h"

void URangeTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);

	ProjectileArc->StartTargeting(InPlayerController);

	auto AbilityComponent = InPlayerController->GetControlledCharacter()->FindComponentByClass<UAbilityComponent>();
	if (ensure(AbilityComponent))
	{
		MoveTarget = Cast<UMoveTarget>(AbilityComponent->GetMoveAbility()->GetTargetState());
	}

	if (ensure(MoveTarget))
	{
		MoveTarget->StartTargeting(InPlayerController);
	}
}

void URangeTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	Target.Location = CursorHitResult.Location;
	if (IsDetected(CursorHitResult.GetActor()))
	{
		MoveTarget->StopTargeting();
		ProjectileArc->TickTargetAbility(CursorHitResult);
		Target.Actor = CursorHitResult.GetActor();
		bEnableRotating = true;
	}
	else
	{
		Target.Actor = nullptr;
		bEnableRotating = false;
		ProjectileArc->StopTargeting();
		MoveTarget->TickTargetAbility(CursorHitResult);
	}
}

void URangeTarget::StopTargeting()
{
	Super::StopTargeting();

	ProjectileArc->StopTargeting();
	MoveTarget->StopTargeting();
}
