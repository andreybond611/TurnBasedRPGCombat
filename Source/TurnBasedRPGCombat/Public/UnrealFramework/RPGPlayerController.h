// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/UITypes.h"
#include "RPGPlayerController.generated.h"

class UAbilityInputConfigData;
struct FInputActionValue;
class UNavigationPath;
class UAbilityTargetState;
class UInventoryWidget;
class UTargetInfoWidget;
class IInteractable;
class UHUDWidget;
class ARPGPlayerState;
class ARPGCharacter;
class ARPGCharacter;
class UPathFollowingComponent;
class UInputMappingContext;
class UInputAction;

DECLARE_STATS_GROUP(TEXT("TargetAbility"), STATGROUP_TargetAbility, STATCAT_Advanced);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnControlledCharacterChanged, ARPGCharacter*, CurrentControlledCharacter, ARPGCharacter*,
											 PreviousControlledCharacter);

/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARPGPlayerController();

	void SetControlledCharacter(ARPGCharacter* Character);
	ARPGCharacter* GetControlledCharacter() const { return ControlledCharacter; }

	void SetIsTargetAllowed(bool bInTargetAllowed) { bTargetAllowed = bInTargetAllowed; }

	void ShowTargetInfo();
	void ResetTargetInfo();
	void AddTargetInfoSection(const FText& Text, int32 Priority, FColor Color = FColor::White);
	void AddDistanceTargetInfoSection(float Distance);
	void AddTargetInfoSectionAccuracy(float Accuracy);
	void AddTargetInfoSectionAPCost(float APCost);
	void AddTargetInfoSectionDistance(float PathDistance);
	void AddTargetInfoSectionCantReachDestination();

	void SetAbilityTargetState(UAbilityTargetState* InAbilityTarget);
	void ChangeInputForReadiableAbilities();
	void ChangeInputForPrimaryAbilities();

	UPROPERTY(BlueprintAssignable)
	FOnControlledCharacterChanged OnControlledCharacterChanged;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	void TargetAbility();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMouseMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* AbilityMouseMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* AbilityMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UAbilityInputConfigData* AbilityInputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Classes", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UHUDWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Classes", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UTargetInfoWidget> TargetInfoWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Settings", meta = (AllowPrivateAccess = "true"))
	FVector2D TargetInfoSize = {200.f, 100.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Settings", meta = (AllowPrivateAccess = "true"))
	FVector2D TargetInfoOffset = {30.f, 30.f};

	TArray<FPriorityText> TargetInfoSections;
	bool bTargetAllowed = true;

	UPROPERTY()
	UHUDWidget* HUDWidget;
	UPROPERTY()
	UTargetInfoWidget* TargetInfoWidget;
	UPROPERTY()
	ARPGPlayerState* RPGPlayerState;
	UPROPERTY()
	ARPGCharacter* ControlledCharacter;
	UPROPERTY()
	UAbilityTargetState* AbilityTarget;
	UPROPERTY()
	ARPGCharacter* CharacterUnderCursor;

	void MouseOverCharacters();
	void SetControlledCharacterPrimaryAbilityTarget();

	UFUNCTION()
	void SetFirstAddedControlledCharacter(ARPGCharacter* AddedCharacter);

	// Input callbacks

	void ExecutePrimaryAbility();
	void SetDestination();
	void SetDestinationTriggered(const FInputActionValue& InputActionValue);
	void ReadyAbility(int32 AbilityIndex);
	void ExecuteReadyAbility();
	void CancelReadyAbility();
};

