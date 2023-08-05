// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/AbilityTargetState.h"

#include "NavigationSystem.h"
#include "Characters/RPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UnrealFramework/RPGPlayerController.h"

void UAbilityTargetState::StartTargeting(ARPGPlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
	ControlledCharacter = PlayerController->GetControlledCharacter();
	AbilityOwner = ControlledCharacter;
}

void UAbilityTargetState::TickTargetAbility(const FHitResult& CursorHitResult)
{
}

void UAbilityTargetState::StopTargeting()
{
}

void UAbilityTargetState::TargetNotHit()
{
	PlayerController->AddTargetInfoSection(NSLOCTEXT("UI", "PathIsBlocked", "Path is Blocked"), 0, FColor::Red);
}

bool UAbilityTargetState::IsHitLocationWalkable(const FHitResult& HitResult)
{
	auto* CharacterMovement = PlayerController->GetControlledCharacter()->GetCharacterMovement();
	return CharacterMovement->IsWalkable(HitResult);
}

void UAbilityTargetState::EnableOutline(AActor* Actor, EOutlineColor Color)
{
	if (auto Outlinable = Cast<IOutlinable>(Actor))
	{
		Outlinable->EnableOutline(Color);

		TScriptInterface<IOutlinable> OutlinableInterace;
		OutlinableInterace.SetInterface(Outlinable);
		OutlinableInterace.SetObject(Actor);
		ActorsWithEnabledOutline.Add(OutlinableInterace);
	}
}

void UAbilityTargetState::EnableOutline(IOutlinable* Actor, EOutlineColor Color)
{
	if (Actor)
	{
		Actor->EnableOutline(Color);

		TScriptInterface<IOutlinable> OutlinableInterace;
		OutlinableInterace.SetInterface(Actor);
		OutlinableInterace.SetObject(Actor->_getUObject());
		ActorsWithEnabledOutline.Add(OutlinableInterace);
	}
}

void UAbilityTargetState::DisableOutline(AActor* Actor)
{
	if (auto Outlinable = Cast<IOutlinable>(Actor))
	{
		Outlinable->DisableOutline();
		ActorsWithEnabledOutline.RemoveAll([Outlinable](const TScriptInterface<IOutlinable>& Item)
		{
			return Item.GetInterface() == Outlinable;
		});
	}
}

void UAbilityTargetState::DisableOutline(IOutlinable* Actor)
{
	if (Actor)
	{
		Actor->DisableOutline();
		ActorsWithEnabledOutline.RemoveAll([Actor](const TScriptInterface<IOutlinable>& Item)
		{
			return Item.GetInterface() == Actor;
		});
	}
}

void UAbilityTargetState::DisableAllOutlines()
{
	for (auto Actor : ActorsWithEnabledOutline)
	{
		Actor->DisableOutline();
	}
	ActorsWithEnabledOutline.Empty();
}

FTarget UAbilityTargetState::GetTarget()
{
	return Target;
}
