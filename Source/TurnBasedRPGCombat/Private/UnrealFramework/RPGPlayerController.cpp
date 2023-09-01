// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "UnrealFramework/RPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Abilities/AbilityComponent.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "Abilities/TargetTypes/MultiTarget.h"
#include "ActorComponents/TurnBasedComponent.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Input/AbilityInputConfigData.h"
#include "UI/HUD/HUDWidget.h"
#include "UI/TargetInfoWidget.h"
#include "UI/UITypes.h"
#include "UI/HUD/HUDComponents/ActionPointsBarWidget.h"
#include "UnrealFramework/RPGPlayerState.h"

DECLARE_CYCLE_STAT(TEXT("TargetAbility"), STAT_TargetAbility, STATGROUP_TargetAbility);

ARPGPlayerController::ARPGPlayerController()
{
}

void ARPGPlayerController::SetDestination()
{
	if (!GetControlledCharacter() || !GetControlledCharacter()->GetTurnBasedComponent()->IsCurrentTurn())
	{
		return;
	}

	ExecutePrimaryAbility();
}

void ARPGPlayerController::ExecutePrimaryAbility()
{
	ControlledCharacter->GetAbilityComponent()->ExecutePrimaryAbility();
}

void ARPGPlayerController::SetControlledCharacter(ARPGCharacter* InCharacter)
{
	if (!InCharacter)
	{
		ControlledCharacter = nullptr;
		SetAbilityTargetState(nullptr);
		return;
	}

	if (ControlledCharacter != InCharacter)
	{
		ControlledCharacter = InCharacter;
		ControlledCharacter->GetAbilityComponent()->Initialize();
		UAbilityTargetState* PrimaryAbilityTarget = ControlledCharacter->GetAbilityComponent()->GetPrimaryAbilityTarget();
		SetAbilityTargetState(PrimaryAbilityTarget);
		ARPGCharacter* PreviousControlledCharacter = ControlledCharacter;
		OnControlledCharacterChanged.Broadcast(ControlledCharacter, PreviousControlledCharacter);
	}
}

void ARPGPlayerController::AddTargetInfoSection(const FText& Text, const int32 Priority, const FColor Color)
{
	const FPriorityText Section = FPriorityText{Text, Priority, Color};
	TargetInfoSections.Add(Section);
}

void ARPGPlayerController::AddDistanceTargetInfoSection(const float Distance)
{
	FNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 1;
	const FText DistanceText = FText::Format(NSLOCTEXT("UI", "DistanceMeters", "{0}m"), FText::AsNumber(Distance, &Options));
	AddTargetInfoSection(DistanceText, 2);
}

void ARPGPlayerController::AddTargetInfoSectionAccuracy(const float Accuracy)
{
	const FText AccuracyText = FText::Format(NSLOCTEXT("UI", "Accuracy", "{0}%"), FText::AsNumber(Accuracy));
	AddTargetInfoSection(AccuracyText, 1);
}

void ARPGPlayerController::AddTargetInfoSectionAPCost(const float APCost)
{
	const float CurrentAP = GetControlledCharacter()->Stats()->Get(SN_ActionPoints);
	if (APCost > CurrentAP)
	{
		AddTargetInfoSection(NSLOCTEXT("UI", "NotEnoughAP", "Not enough action points"), 0);
	}
	else
	{
		const FText APCostText = FText::Format(NSLOCTEXT("UI", "ActionPointCost", "{0}AP"), FText::AsNumber(APCost));
		AddTargetInfoSection(APCostText, 1);
	}

	UActionPointsBarWidget* APBar = HUDWidget->GetAPBar();
	if (APBar)
	{
		APBar->SetSpendingIndication(FMath::RoundToInt(APCost));
	}
}

void ARPGPlayerController::AddTargetInfoSectionDistance(const float PathDistance)
{
	const float DistanceMeters = PathDistance / 100.f;
	FNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 1;
	const FText Text = FText::Format(NSLOCTEXT("UI", "DistanceMeters", "{0}m"), FText::AsNumber(DistanceMeters, &Options));

	AddTargetInfoSection(Text, 3);
}

void ARPGPlayerController::SetAbilityTargetState(UAbilityTargetState* InAbilityTarget)
{
	if (AbilityTarget)
	{
		AbilityTarget->StopTargeting();
	}
	AbilityTarget = InAbilityTarget;
	if (AbilityTarget)
	{
		AbilityTarget->StartTargeting(this);
	}
}

void ARPGPlayerController::SetReadiableAbilitiesInput()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AbilityMouseMappingContext, AbilityInputPriority);
	}

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(MultiTargetMouseMappingContext);
	}
}

void ARPGPlayerController::SetPrimaryAbilitiesInput()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(AbilityMouseMappingContext);
	}

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(MultiTargetMouseMappingContext);
	}
}

void ARPGPlayerController::SetMultiTargetInput()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MultiTargetMouseMappingContext, MultiAbilityTargetInputPriority);
	}
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UHUDWidget>(this, *HUDWidgetClass);
	HUDWidget->Init(this);
	HUDWidget->AddToViewport();

	RPGPlayerState = GetPlayerState<ARPGPlayerState>();
	check(RPGPlayerState);

	if (!RPGPlayerState->GetPlayerParty().IsEmpty())
	{
		SetControlledCharacter(RPGPlayerState->GetPlayerParty()[0]);
		check(ControlledCharacter)
	}
	else
	{
		RPGPlayerState->OnCharacterAddedToParty.AddDynamic(this, &ARPGPlayerController::SetFirstAddedControlledCharacter);
	}
}

void ARPGPlayerController::ShowTargetInfo()
{
	TargetInfoWidget = CreateWidget<UTargetInfoWidget>(this, *TargetInfoWidgetClass);

	TargetInfoWidget->Init(TargetInfoSections);

	float X, Y;
	GetMousePosition(X, Y);

	FVector2D ViewportPosition;
	USlateBlueprintLibrary::ScreenToViewport(this, FVector2D{X, Y}, ViewportPosition);

	TargetInfoWidget->SetPositionInViewport(ViewportPosition + TargetInfoOffset, false);
	TargetInfoWidget->SetDesiredSizeInViewport(TargetInfoSize);
	TargetInfoWidget->AddToViewport();
}

void ARPGPlayerController::ResetTargetInfo()
{
	if (TargetInfoWidget)
	{
		TargetInfoWidget->RemoveFromParent();
		TargetInfoWidget = nullptr;
	}
	TargetInfoSections.Empty();
}

void ARPGPlayerController::AddTargetInfoSectionCantReachDestination()
{
	const FText Message = NSLOCTEXT("UI", "CannotReachDestination", "Can't reach destination");
	AddTargetInfoSection(Message, 0, FColor::Red);
	AddDistanceTargetInfoSection(0.f);
}

void ARPGPlayerController::MouseOverCharacters()
{
	FHitResult HitResult;
	const bool bHit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1 /*MouseCursor*/, false, HitResult);
	if (bHit)
	{
		if (const auto HitCharacter = Cast<ARPGCharacter>(HitResult.GetActor()))
		{
			if (HitCharacter != ControlledCharacter)
			{
				HUDWidget->SetMouseOverCharacter(HitCharacter);
				CharacterUnderCursor = HitCharacter;
				return;
			}
		}
	}

	HUDWidget->SetMouseOverCharacter(nullptr);
}

void ARPGPlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!GetControlledCharacter())
	{
		return;
	}

	MouseOverCharacters();

	if (GetControlledCharacter()->GetTurnBasedComponent()->IsCurrentTurn())
	{
		TargetAbility();
	}
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMouseMappingContext, 0);
		Subsystem->AddMappingContext(AbilityMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SetDestinationInput, ETriggerEvent::Started, this, &ARPGPlayerController::SetDestination);
		EnhancedInputComponent->BindAction(SetDestinationInput, ETriggerEvent::Triggered, this, &ARPGPlayerController::SetDestinationTriggered);

		EnhancedInputComponent->BindAction(AbilityInputConfig->AbilityInput1, ETriggerEvent::Started, this, &ARPGPlayerController::ReadyAbility, 0);
		EnhancedInputComponent->BindAction(AbilityInputConfig->AbilityInput2, ETriggerEvent::Started, this, &ARPGPlayerController::ReadyAbility, 1);
		EnhancedInputComponent->BindAction(AbilityInputConfig->AbilityInput3, ETriggerEvent::Started, this, &ARPGPlayerController::ReadyAbility, 2);
		EnhancedInputComponent->BindAction(AbilityInputConfig->AbilityInput4, ETriggerEvent::Started, this, &ARPGPlayerController::ReadyAbility, 3);

		EnhancedInputComponent->BindAction(AbilityInputConfig->ExecuteReadyAbility, ETriggerEvent::Started, this,
										   &ARPGPlayerController::ExecuteReadyAbility);
		EnhancedInputComponent->BindAction(AbilityInputConfig->CancelReadyAbility, ETriggerEvent::Started, this,
										   &ARPGPlayerController::CancelReadyAbility);

		EnhancedInputComponent->BindAction(AbilityInputConfig->ConfirmTarget, ETriggerEvent::Started, this, &ARPGPlayerController::ConfirmTarget);
		EnhancedInputComponent->BindAction(AbilityInputConfig->CancelConfirmedTarget, ETriggerEvent::Started, this,
										   &ARPGPlayerController::CancelConfirmedTarget);
	}
}

void ARPGPlayerController::ReadyAbility(const int32 AbilityIndex)
{
	if (ControlledCharacter)
	{
		ControlledCharacter->ReadyAbility(AbilityIndex);
	}
}

void ARPGPlayerController::ExecuteReadyAbility()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->ExecuteReadyAbility();
	}
}

void ARPGPlayerController::SetControlledCharacterPrimaryAbilityTarget()
{
	UAbilityComponent* ControlledAbilityComponent = GetControlledCharacter()->GetAbilityComponent();
	UAbilityTargetState* PrimaryAbilityTarget = ControlledAbilityComponent->GetPrimaryAbilityTarget();
	SetAbilityTargetState(PrimaryAbilityTarget);
}

void ARPGPlayerController::CancelReadyAbility()
{
	SetControlledCharacterPrimaryAbilityTarget();
	GetControlledCharacter()->UnReadyCurrentAbility();
}

void ARPGPlayerController::ConfirmTarget()
{
	if (auto MultiTarget = Cast<UMultiTarget>(AbilityTarget))
	{
		if (MultiTarget->CanConfirmTarget())
		{
			MultiTarget->ConfirmTarget();
		}
	}
}

void ARPGPlayerController::CancelConfirmedTarget()
{
	if (auto MultiTarget = Cast<UMultiTarget>(AbilityTarget))
	{
		MultiTarget->CancelConfirmedTarget();
	}
}

void ARPGPlayerController::TargetAbility()
{
	SCOPE_CYCLE_COUNTER(STAT_TargetAbility);

	if (AbilityTarget && ControlledCharacter)
	{
		ResetTargetInfo();

		FHitResult HitResult;
		const bool bHit = GetHitResultUnderCursor(AbilityTarget->GetCollisionChannel(), false, HitResult);
		if (bHit)
		{
			AbilityTarget->TickTargetAbility(HitResult);
			if (AbilityTarget->GetEnableRotating())
			{
				GetControlledCharacter()->RotateToFaceLocation(HitResult.Location);
			}
			else
			{
				GetControlledCharacter()->StopRotating();
			}
		}
		else
		{
			AbilityTarget->TargetNotHit();
		}
		ShowTargetInfo();
	}
}

void ARPGPlayerController::SetFirstAddedControlledCharacter(ARPGCharacter* AddedCharacter)
{
	SetControlledCharacter(AddedCharacter);
	RPGPlayerState->OnCharacterAddedToParty.RemoveDynamic(this, &ARPGPlayerController::SetFirstAddedControlledCharacter);
}

void ARPGPlayerController::SetDestinationTriggered(const FInputActionValue& InputActionValue)
{
	if (ControlledCharacter && !ControlledCharacter->GetTurnBasedComponent()->IsCurrentTurn())
	{
		const bool bValue = InputActionValue.Get<bool>();

		FHitResult OutHit;
		if (bValue && GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1 /*MouseCursor*/, false, OutHit))
		{
			ControlledCharacter->MoveToLocation(OutHit.Location);
		}
	}
}
