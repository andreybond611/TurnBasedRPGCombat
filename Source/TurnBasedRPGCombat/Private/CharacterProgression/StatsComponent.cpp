// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "CharacterProgression/StatsComponent.h"
#include "CharacterProgression/GameStat.h"

void UStatsComponent::InitializeStats()
{
}

void UStatsComponent::SetStatAsMaxStat(const FName StatName, const FName MaxStatName)
{
	UGameStat* Stat = FindStat(StatName);
	UGameStat* MaxStat = FindStat(MaxStatName);

	if (ensureMsgf(Stat && MaxStat, TEXT("%s Stat and %s MaxStat aren't set in %s"), *StatName.ToString(), *MaxStatName.ToString(), *GetName()))
	{
		Stat->SetStatAsMaxValue(MaxStat);
	}
}

UGameStat* UStatsComponent::FindStat(const FName StatName)
{
	if (Stats.Contains(StatName))
	{
		return Stats[StatName];
	}
	return nullptr;
}

float UStatsComponent::Get(const FName StatName)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName());
		return Stats[StatName]->Get();
	}
	return 0.f;
}

void UStatsComponent::Set(const FName StatName, const float Value)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName());
		Stats[StatName]->SetValue(Value);
	}
}

void UStatsComponent::SetConstant(const FName StatName, const float Value)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName());
		Stats[StatName]->SetConstant(Value);
	}
}

void UStatsComponent::Add(const FName StatName, const float Value)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName());
		Stats[StatName]->Add(Value);
	}
}

void UStatsComponent::Remove(const FName StatName, const float Value)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName());
		Stats[StatName]->Remove(Value);
	}
}

UGameStat* UStatsComponent::CreateStat(const FName StatName, const FText TextName, const float InitialValue, const float MinValue, const float MaxValue)
{
	UGameStat* CreatedStat = UGameStat::Create(InitialValue, TextName, MinValue, MaxValue);
	Stats.Add(StatName, CreatedStat);
	return CreatedStat;
}

void UStatsComponent::AddMultiplier(const FName StatName, const float Multiplier)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName());
		Stats[StatName]->AddMultiplier(Multiplier);
	}
}

void UStatsComponent::AddStatToCollection(const FName StatName, UGameStat* Stat)
{
	if (StatName != NAME_None && Stat != nullptr)
	{
		Stats.Add(StatName, Stat);
	}
}

void UStatsComponent::DeleteStatAt(const FName StatName)
{
	Stats.Remove(StatName);
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeStats();
}
