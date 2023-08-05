// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "CharacterProgression/GameStat.h"
#include "RPGAnimInstance.generated.h"



USTRUCT()
struct FAbilityStateAnimations
{
	GENERATED_BODY()
public:
	FAbilityStateAnimations() = default;

	UPROPERTY()
	UAnimSequence* AbilityStart = nullptr;
	UPROPERTY()
	UAnimSequence* AbilityReady = nullptr;
	UPROPERTY()
	UAnimSequence* AbilityCancel = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagsChanged, const FGameplayTagContainer&, GameplayTags);

/**
 *
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void SetAbilityAnimations(const FAbilityStateAnimations& AbilityAnimations);
	virtual void NativeInitializeAnimation() override;
	void SetMovementAnimRate(float Rate);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnGameplayTagsChanged OnGameplayTagsChanged;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AbilityStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AbilityReady;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AbilityCancel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementAnimRate = 1.f;

	UFUNCTION()
	void GameplayTagChanged(const FGameplayTagContainer& GameplayTags);

	UPROPERTY(BlueprintReadOnly, Category = "Tags", meta = (AllowPrivateAccess = "true"))
	bool bStunt;
};
