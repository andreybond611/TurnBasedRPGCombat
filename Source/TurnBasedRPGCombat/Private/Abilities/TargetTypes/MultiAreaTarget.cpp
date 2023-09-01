// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/MultiAreaTarget.h"
#include "Abilities/TargetTypes/ProjectileRadiusTarget.h"
#include "Abilities/TargetTypes/Helpers/AOESphereActor.h"
#include "UnrealFramework/RPGPlayerController.h"

void UMultiAreaTarget::ConfirmTarget()
{
	++TargetNum;
	const FVector TargetLocation = SingleAbilityTarget->GetTarget().Location;
	Target.MultipleLocations.Add(TargetLocation);

	if (TargetNum == MaxTargetNum)
	{
		PlayerController->ExecuteReadyAbility();
		TargetNum = 0;
		return;
	}
	
	const FRotator TargetRotation;
	auto* ConfirmedTargetActor = GetWorld()->SpawnActor<AAOESphereActor>(*ConfirmedTargetClass, TargetLocation, TargetRotation);
	if (ensure(ConfirmedTargetActor))
	{
		ConfirmedTargetActors.Add(ConfirmedTargetActor);
		if (auto ProjectileRadiusTarget = Cast<UProjectileRadiusTarget>(SingleAbilityTarget))
		{
			ProjectileRadiusTarget->SetSphereRadius(ConfirmedTargetActor);
		}
	}
}

void UMultiAreaTarget::CancelConfirmedTarget()
{
	if (TargetNum == 0)
	{
		PlayerController->CancelReadyAbility();
		return;
	}

	--TargetNum;

	const int32 LastIndex = ConfirmedTargetActors.Num() - 1;

	check(ConfirmedTargetActors.IsValidIndex(LastIndex) && ConfirmedTargetActors[LastIndex]);

	ConfirmedTargetActors.Last()->Destroy();
	ConfirmedTargetActors.RemoveAt(LastIndex);
}

void UMultiAreaTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);
}

void UMultiAreaTarget::StopTargeting()
{
	Super::StopTargeting();

	for (auto Actor : ConfirmedTargetActors)
	{
		Actor->Destroy();
	}

	ConfirmedTargetActors.Empty();
}
