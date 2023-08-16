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

void UGameStat::SetValue(float InValue)
{
	ActualValue = FMath::Clamp(InValue, MinValue, MaxValue);
	OverflowValue = ActualValue;

	ValueForEdit = ActualValue;

	OnChange.Broadcast(ActualValue);
}

UGameStat* UGameStat::Create(float InInitialValue, FText InName, float InMinValue, float InMaxValue)
{
	// todo: pass fname here
	FName StatName = MakeUniqueObjectName(GetTransientPackage(), StaticClass(), FName(InName.ToString()));

	UGameStat* NewStat = NewObject<UGameStat>(GetTransientPackage(), StatName);
	NewStat->MaxValue = InMaxValue;
	NewStat->MinValue = InMinValue;
	NewStat->Name = InName;
	NewStat->SetValue(InInitialValue);
	return NewStat;
}

void UGameStat::Add(float Value)
{
	if (Value < 0.f)
	{
		UE_LOG(LogStat, Warning, TEXT("Negative value added to %s"), *GetName())
		return;
	}

	if (Value != 0.f) //-V550
	{
		float PreviousValue = ActualValue;

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

void UGameStat::Remove(float Value)
{
	if (Value < 0.f)
	{
		UE_LOG(LogStat, Warning, TEXT("Negative value removed to %s"), *GetName())
		return;
	}

	if (Value != 0.f) //-V550
	{
		float PreviousValue = ActualValue;

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

void UGameStat::AddMultiplier(float InMultiplier)
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

	FName StatPropertyName = PropertyChangedEvent.GetPropertyName();
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

