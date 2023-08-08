// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "CharacterProgression/StatsComponent.h"
#include "CharacterProgression/GameStat.h"

void UStatsComponent::InitializeStats()
{
}

void UStatsComponent::SetStatAsMaxStat(FName StatName, FName MaxStatName)
{
	UGameStat* Stat = FindStat(StatName);
	UGameStat* MaxStat = FindStat(MaxStatName);

	if (ensureMsgf(Stat && MaxStat, TEXT("%s Stat and %s MaxStat aren't set in %s"), *StatName.ToString(), *MaxStatName.ToString(), *GetName()))
	{
		Stat->SetStatAsMaxValue(MaxStat);
	}
}

UGameStat* UStatsComponent::FindStat(FName StatName)
{
	if (Stats.Contains(StatName))
	{
		return Stats[StatName];
	}
	return nullptr;
}

float UStatsComponent::Get(FName StatName)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName()) return Stats[StatName]->Get();
	}
	return 0.f;
}

void UStatsComponent::Set(FName StatName, float Value)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName()) Stats[StatName]->SetValue(Value);
	}
}

void UStatsComponent::Add(FName StatName, float Value)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName()) Stats[StatName]->Add(Value);
	}
}

void UStatsComponent::Remove(FName StatName, float Value)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName()) Stats[StatName]->Remove(Value);
	}
}

UGameStat* UStatsComponent::CreateStat(FName StatName, FText TextName, float InitialValue, float MinValue, float MaxValue)
{
	UGameStat* CreatedStat = UGameStat::Create(InitialValue, TextName, MinValue, MaxValue);
	Stats.Add(StatName, CreatedStat);
	return CreatedStat;
}

void UStatsComponent::AddMultiplier(FName StatName, float Multiplier)
{
	if (Stats.Contains(StatName))
	{
		checkf(Stats[StatName], TEXT("Stat %s is not set. Owner: %s"), *StatName.ToString(), *GetOwner()->GetName()) Stats[StatName]->AddMultiplier(
			Multiplier);
	}
}

void UStatsComponent::AddStatToCollection(FName StatName, UGameStat* Stat)
{
	if (StatName != NAME_None && Stat != nullptr)
	{
		Stats.Add(StatName, Stat);
	}
}

void UStatsComponent::DeleteStatAt(FName StatName)
{
	Stats.Remove(StatName);
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeStats();
}
