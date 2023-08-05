// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Characters/RPGCharacter.h"
#include "Components/ActorComponent.h"
#include "UnrealFramework/RPGPlayerController.h"
#include "AbilityComponent.generated.h"

class USimpleAbility;
class UReadiableAbility;
class UAbilityTargetState;
class UEffect;
class URPGAnimInstance;
class UAbility;
class UMoveAbility;
/**
 * Manages ability execution
 */
UCLASS(ClassGroup = (Ability), meta = (BlueprintSpawnableComponent))
class TURNBASEDRPGCOMBAT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	UFUNCTION(BlueprintCallable)
	void ReadyAbility(UReadiableAbility* Ability);
	UFUNCTION(BlueprintCallable)
	void ReadyAbilityAt(int32 Index);
	UFUNCTION(BlueprintCallable)
	void CancelReadyAbility();

	bool CanReadyAbility(UReadiableAbility* Ability);

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool IsAbilityReady() const { return CurrentlyReadiedAbility != nullptr; }

	UFUNCTION(BlueprintCallable)
	void ExecuteReadyAbility();
	bool IsPlayerControlled();

	UFUNCTION(BlueprintCallable)
	void ExecuteAbility(UAbility* Ability);

	UAbility* FindAbility(UClass* AbilityClass);

	template <typename AbilityClass>
	AbilityClass* FindAbility(UClass* Class = AbilityClass::StaticClass());

	UReadiableAbility* GetAbilityAt(int32 Index);
	USimpleAbility* GetPrimaryAbility() const { return PrimaryAbility; }
	UAbilityTargetState* GetPrimaryAbilityTarget();
	void ExecutePrimaryAbility();
	TArray<UAbility*> GetAllAbilities();
	void Initialize();
	UMoveAbility* GetMoveAbility()const { return MoveAbility; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Combat parameters", meta = (AllowPrivateAccess = "true"))
	TArray<UReadiableAbility*> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Combat parameters", meta = (AllowPrivateAccess = "true"))
	USimpleAbility* MeleeAttackAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Combat parameters", meta = (AllowPrivateAccess = "true"))
	USimpleAbility* RangeAttackAbility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Combat parameters", meta = (AllowPrivateAccess = "true"))
	UMoveAbility* MoveAbility;

	UPROPERTY()
	USimpleAbility* PrimaryAbility;

	bool bAllowAbilityReady = true;
	bool bInitialized = false;

	UPROPERTY()
	UReadiableAbility* CurrentlyReadiedAbility;
	UPROPERTY()
	ARPGCharacter* Character;
	UPROPERTY()
	URPGAnimInstance* AnimInstance;
	UPROPERTY()
	UAbility* CurrentlyExecutingAbility;
	UPROPERTY()
	ARPGPlayerController* PlayerController;

	FTimerHandle FailSafeHandle;
	float MaxAbilityExecutionTime = 5.f;

	UFUNCTION()
	void OnAbilityDone();

	void SetupPrimaryAbility();
};

template <typename AbilityClass>
AbilityClass* UAbilityComponent::FindAbility(UClass* Class)
{
	return FindAbility(Class);
}
