// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/MeleeTarget.h"

#include "EnhancedInputSubsystems.h"
#include "NavigationSystem.h"
#include "Abilities/Ability.h"
#include "Abilities/AbilityComponent.h"
#include "Abilities/MoveAbility.h"
#include "Abilities/TargetTypes/MoveTarget.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealFramework/RPGPlayerController.h"
#include "Utility/TurnBasedUtility.h"

UMeleeTarget::UMeleeTarget()
{
	bEnableRotating = false;
}

void UMeleeTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);

	auto AbilityComponent =  InPlayerController->GetControlledCharacter()->FindComponentByClass<UAbilityComponent>();
	if (ensure(AbilityComponent))
	{
		MoveTarget = Cast<UMoveTarget>(AbilityComponent->GetMoveAbility()->GetTargetState());
	}

	MoveTarget->StartTargeting(InPlayerController);
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

