// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/TargetTypes/MultiTarget.h"
#include "UnrealFramework/RPGPlayerController.h"

void UMultiTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);

	InPlayerController->SetMultiTargetInput();
	SingleAbilityTarget->StartTargeting(InPlayerController);

	Target = {};
}

void UMultiTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	if (SingleAbilityTarget)
	{
		SingleAbilityTarget->TickTargetAbility(CursorHitResult);
	}
}

void UMultiTarget::StopTargeting()
{
	PlayerController->SetPrimaryAbilitiesInput();
	SingleAbilityTarget->StopTargeting();
}

void UMultiTarget::ConfirmTarget()
{

}

void UMultiTarget::CancelConfirmedTarget()
{

}

bool UMultiTarget::CanConfirmTarget()
{
	return true;
}
