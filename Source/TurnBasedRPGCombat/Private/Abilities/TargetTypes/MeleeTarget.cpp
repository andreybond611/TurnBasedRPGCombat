// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/MeleeTarget.h"

#include "Abilities/Ability.h"
#include "Abilities/AbilityComponent.h"
#include "Abilities/MoveAbility.h"
#include "Abilities/TargetTypes/MoveTarget.h"
#include "Characters/RPGCharacter.h"
#include "UnrealFramework/RPGPlayerController.h"

UMeleeTarget::UMeleeTarget()
{
	bEnableRotating = false;
}

void UMeleeTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);

	const auto AbilityComponent =  InPlayerController->GetControlledCharacter()->FindComponentByClass<UAbilityComponent>();
	if (ensure(AbilityComponent))
	{
		MoveTarget = Cast<UMoveTarget>(AbilityComponent->GetMoveAbility()->GetTargetState());
	}

	if (ensure(MoveTarget))
	{
		MoveTarget->StartTargeting(InPlayerController);
	}
}

void UMeleeTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	MoveTarget->TickTargetAbility(CursorHitResult);
	Target = MoveTarget->GetTarget();
}

void UMeleeTarget::StopTargeting()
{
	Super::StopTargeting();

	MoveTarget->StopTargeting();
}

void UMeleeTarget::TargetNotHit()
{
	PlayerController->AddTargetInfoSectionCantReachDestination();
}

