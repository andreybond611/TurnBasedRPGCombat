// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Utility/TurnBasedTypes.h"
#include "GameStat.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeStat, float /*CurrentValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddStat, float /*AddedValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemoveStat, float /*RemovedValue*/);
DECLARE_MULTICAST_DELEGATE(FOnReachMinValue);
DECLARE_MULTICAST_DELEGATE(FOnReachMaxValue);

/**
 *
 */
UCLASS(BlueprintType, EditInlineNew, Blueprintable)
class TURNBASEDRPGCOMBAT_API UGameStat : public UObject
{
	GENERATED_BODY()
public:
	UGameStat();

	static UGameStat* Create(float InInitialValue, FText InName, float InMinValue = MinStatValue, float InMaxValue = MaxStatValue);

	UFUNCTION(BlueprintPure)
	float Get() const { return ActualValue; }

	void SetValue(float InValue);
	void SetConstant(float InValue);

	UFUNCTION(BlueprintCallable)
	void Add(float Value);

	UFUNCTION(BlueprintCallable)
	void Remove(float Value);

	UFUNCTION(BlueprintCallable)
	void AddMultiplier(float InMultiplier);

	UFUNCTION(BlueprintPure)
	float GetMultiplier() const { return Multiplier; }

	FOnChangeStat OnChange;
	FOnAddStat OnAddStat;
	FOnRemoveStat OnRemoveStat;
	FOnReachMinValue OnReachMinValue;
	FOnReachMaxValue OnReachMaxValue;

	UFUNCTION(BlueprintCallable)
	void SetStatAsMaxValue(UGameStat* InMaxValueStat);
	UFUNCTION(BlueprintCallable)
	void RemoveStatAsMaxValue();

public:
	void SetMaxValue(float InMaxValue) { MaxValue = InMaxValue; }
	void SetMinValue(float InMinValue) { MinValue = InMinValue; }
	void SetMinMaxValue(float InMaxValue, float InMinValue)
	{
		MaxValue = InMaxValue;
		MinValue = InMinValue;
	}

	float GetMaxValue() const { return MaxValue; }
	float GetMinValue() const { return MinValue; }

	UGameStat* GetMaxValueStat() const { return MaxValueStat; }
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
private:
	UPROPERTY(EditAnywhere, DisplayName = "Value")
	float ValueForEdit;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float OverflowValue;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ActualValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxValue = MaxStatValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MinValue = MinStatValue;

	UPROPERTY(EditAnywhere)
	float Multiplier = 1.f;
	float PreviousMultiplier;

	UPROPERTY(EditAnywhere)
	bool bIsConstant = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText Name;

	FDelegateHandle MaxValueStatChangedHandle;

	UPROPERTY()
	UGameStat* MaxValueStat;
};
