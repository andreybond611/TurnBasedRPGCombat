// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/AbilityComponent.h"
#include "Abilities/Ability.h"
#include "Abilities/MoveAbility.h"
#include "Abilities/ReadiableAbility.h"
#include "Abilities/SimpleAbility.h"
#include "Abilities/TargetTypes/NoneAbilityTarget.h"
#include "Characters/RPGCharacter.h"
#include "Characters/Animations/RPGAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UnrealFramework/RPGPlayerController.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::ReadyAbility(UReadiableAbility* Ability)
{
	if (CanReadyAbility(Ability))
	{
		AnimInstance->SetAbilityAnimations(Ability->GetStateAnimations());
		CurrentlyReadiedAbility = Ability;
		Ability->ReadyAbility();

		if (IsPlayerControlled())
		{
			PlayerController->GetControlledCharacter()->GetCharacterMovement()->StopActiveMovement();
			PlayerController->SetReadiableAbilitiesInput();
			PlayerController->SetAbilityTargetState(Ability->GetTargetState());
		}
	}
}

void UAbilityComponent::ReadyAbilityAt(const int32 Index)
{
	ReadyAbility(GetAbilityAt(Index));
}

void UAbilityComponent::CancelReadyAbility()
{
	if (CurrentlyReadiedAbility)
	{
		// todo: call when ability cancel animation ends
		CurrentlyReadiedAbility->CancelReadyAbility();
		CurrentlyReadiedAbility = nullptr;

		if (IsPlayerControlled())
		{
			PlayerController->SetPrimaryAbilitiesInput();
		}
	}
}

bool UAbilityComponent::CanReadyAbility(UReadiableAbility* Ability)
{
	return Ability && Ability->CanBeReadied() && bAllowAbilityReady && CurrentlyReadiedAbility == nullptr;
}

void UAbilityComponent::ExecuteReadyAbility()
{
	if (!CurrentlyReadiedAbility->GetTargetState()->IsAllowedExecute())
	{
		return;
	}

	ExecuteAbility(CurrentlyReadiedAbility);
	CurrentlyReadiedAbility->GetTargetState()->StopTargeting();
	CurrentlyReadiedAbility = nullptr;

	if (IsPlayerControlled())
	{
		PlayerController->SetPrimaryAbilitiesInput();
	}
}

bool UAbilityComponent::IsPlayerControlled()
{
	return PlayerController->GetControlledCharacter() == GetOwner();
}

void UAbilityComponent::ForceEndCurrentAbility()
{
	CurrentlyExecutingAbility->EndAbility();
}

void UAbilityComponent::ExecuteAbility(UAbility* Ability)
{
	Ability->StartAbility();

	if (IsPlayerControlled())
	{
		PlayerController->DisableInput(nullptr);
		PlayerController->SetAbilityTargetState(nullptr);
	}
	CurrentlyExecutingAbility = Ability;
	CurrentlyExecutingAbility->OnAbilityDoneExecuting.AddDynamic(this, &UAbilityComponent::OnAbilityDone);

	GetWorld()->GetTimerManager().SetTimer(FailSafeHandle, this, &UAbilityComponent::ForceEndCurrentAbility, MaxAbilityExecutionTime);
}

UAbility* UAbilityComponent::FindAbility(UClass* AbilityClass)
{
	for (UReadiableAbility* Ability : Abilities)
	{
		if (Ability->IsA(AbilityClass))
		{
			return Ability;
		}
	}
	return nullptr;
}

UReadiableAbility* UAbilityComponent::GetAbilityAt(const int32 Index)
{
	return Abilities.IsValidIndex(Index) ? Abilities[Index] : nullptr;
}

void UAbilityComponent::SetupPrimaryAbility()
{
	if (ensureMsgf(MoveAbility, TEXT("%s doesn't have move ability"), *GetOwner()->GetName()))
	{
		MoveAbility->AddToCharacter(Character);
	}

	if (RangeAttackAbility)
	{
		PrimaryAbility = RangeAttackAbility;
	}
	else
	{
		checkf(MeleeAttackAbility, TEXT("No primary ability selected for %s"), *GetOwner()->GetName()) PrimaryAbility = MeleeAttackAbility;
	}

	PrimaryAbility->AddToCharacter(Character);
}

void UAbilityComponent::OnAbilityDone()
{
	if (PlayerController->GetControlledCharacter() == GetOwner())
	{
		PlayerController->EnableInput(nullptr);
		PlayerController->SetAbilityTargetState(PrimaryAbility->GetTargetState());
	}

	CurrentlyExecutingAbility->OnAbilityDoneExecuting.RemoveDynamic(this, &UAbilityComponent::OnAbilityDone);
	CurrentlyExecutingAbility = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(FailSafeHandle);
}

UAbilityTargetState* UAbilityComponent::GetPrimaryAbilityTarget()
{
	return PrimaryAbility->GetTargetState();
}

void UAbilityComponent::ExecutePrimaryAbility()
{
	ExecuteAbility(PrimaryAbility);
}

TArray<UAbility*> UAbilityComponent::GetAllAbilities()
{
	TArray<UAbility*> Result;
	Result.Append(Abilities);
	if (MeleeAttackAbility)
	{
		Result.Add(MeleeAttackAbility);
	}
	if (RangeAttackAbility)
	{
		Result.Add(RangeAttackAbility);
	}

	return Result;
}

void UAbilityComponent::Initialize()
{
	if (!bInitialized)
	{
		bInitialized = true;

		// todo: too much dependent on character
		Character = Cast<ARPGCharacter>(GetOwner());
		if (Character)
		{
			AnimInstance = Cast<URPGAnimInstance>(Character->GetMesh()->GetAnimInstance());
		}

		PlayerController = GetWorld()->GetFirstPlayerController<ARPGPlayerController>();
		checkf(PlayerController, TEXT("ARPGPlayerContrller isn't set"));

		SetupPrimaryAbility();

		/*if (auto TurnBasedBattleParticipant = Cast<ITurnBasedBattleParticipant>(GetOwner()))
		{
			if (TurnBasedBattleParticipant->IsPlayerControlled())
			{*/
		for (UReadiableAbility* Ability : Abilities)
		{
			Ability->AddToCharacter(Character);
		}
		/*	}
		}*/
	}
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void UAbilityComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
