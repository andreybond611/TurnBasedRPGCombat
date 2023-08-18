// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "CharacterProgression/GameStat.h"

DEFINE_LOG_CATEGORY_STATIC(LogStat, All, All)

UGameStat::UGameStat()
{
	PreviousMultiplier = Multiplier;
	ActualValue = FMath::Clamp(0.f, MinValue, MaxValue);
	OverflowValue = ActualValue;

	ValueForEdit = ActualValue;
}

void UGameStat::SetValue(const float InValue)
{
	if (!bIsConstant)
	{
		ActualValue = FMath::Clamp(InValue, MinValue, MaxValue);
		OverflowValue = ActualValue;

		ValueForEdit = ActualValue;

		OnChange.Broadcast(ActualValue);
	}
}

void UGameStat::SetConstant(const float InValue)
{
	if (bIsConstant)
	{
		bIsConstant = false;
	}
	SetValue(InValue);
	bIsConstant = true;
}

UGameStat* UGameStat::Create(const float InInitialValue, FText InName, const float InMinValue, const float InMaxValue)
{
	// todo: pass FName instead of converting FText
	const FName StatName = MakeUniqueObjectName(GetTransientPackage(), StaticClass(), FName(InName.ToString()));

	UGameStat* NewStat = NewObject<UGameStat>(GetTransientPackage(), StatName);
	NewStat->MaxValue = InMaxValue;
	NewStat->MinValue = InMinValue;
	NewStat->Name = InName;
	NewStat->SetValue(InInitialValue);
	return NewStat;
}

void UGameStat::Add(const float Value)
{
	if (Value < 0.f)
	{
		UE_LOG(LogStat, Warning, TEXT("Negative value added to %s"), *GetName())
		return;
	}

	if (Value != 0.f && !bIsConstant) //-V550
	{
		const float PreviousValue = ActualValue;

		OverflowValue = OverflowValue + Value;
		ActualValue = FMath::Clamp(OverflowValue, MinValue, MaxValue);

		ValueForEdit = ActualValue;

		if (PreviousValue != ActualValue) //-V550
		{
			OnChange.Broadcast(ActualValue);
			OnAddStat.Broadcast(Value);

			if (FMath::IsNearlyEqual(ActualValue, MaxValue))
			{
				OnReachMaxValue.Broadcast();
			}
		}
	}
}

void UGameStat::Remove(const float Value)
{
	if (Value < 0.f)
	{
		UE_LOG(LogStat, Warning, TEXT("Negative value removed to %s"), *GetName())
		return;
	}

	if (Value != 0.f && !bIsConstant) //-V550
	{
		const float PreviousValue = ActualValue;

		OverflowValue = OverflowValue - Value;
		ActualValue = FMath::Clamp(OverflowValue, MinValue, MaxValue);

		ValueForEdit = ActualValue;

		if (PreviousValue != ActualValue) //-V550
		{
			OnChange.Broadcast(ActualValue);
			OnRemoveStat.Broadcast(Value);

			if (FMath::IsNearlyEqual(ActualValue, MinValue))
			{
				OnReachMinValue.Broadcast();
			}
		}
	}
}

void UGameStat::AddMultiplier(const float InMultiplier)
{
	OverflowValue /= PreviousMultiplier;
	Multiplier = FMath::Clamp<float>(Multiplier + InMultiplier, 0.f, 9999.f);
	PreviousMultiplier = Multiplier;
	SetValue(OverflowValue * Multiplier);
}

void UGameStat::SetStatAsMaxValue(UGameStat* InMaxValueStat)
{
	MaxValueStat = InMaxValueStat;
	MaxValueStatChangedHandle = MaxValueStat->OnChange.AddUObject(this, &UGameStat::SetMaxValue);
	MaxValue = MaxValueStat->Get();
	SetValue(Get());
}

void UGameStat::RemoveStatAsMaxValue()
{
	MaxValueStat->OnChange.Remove(MaxValueStatChangedHandle);
}

#if WITH_EDITOR
void UGameStat::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	const FName StatPropertyName = PropertyChangedEvent.GetPropertyName();
	if (StatPropertyName == GET_MEMBER_NAME_CHECKED(UGameStat, ValueForEdit))
	{
		SetValue(ValueForEdit);
	}
	if (StatPropertyName == GET_MEMBER_NAME_CHECKED(UGameStat, Multiplier))
	{
		AddMultiplier(0.f);
	}
	
}
#endif

