// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/RangeAIAbilityTarget.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "AIController.h"
#include "Abilities/Ability.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/MovementComponent.h"
#include "TurnBased/TurnBasedBattle.h"
#include "TurnBased/TurnBasedBattleParticipant.h"

DEFINE_LOG_CATEGORY_STATIC(LogRangeAIAbilityTarget, All, All)

FRangeAttackLocations FRangeAttackLocations::Invalid = {false};

void FRangeAttackLocations::SetDesirability(double InDesirability)
{
	if (InDesirability > HighestDesirability)
	{
		HighestDesirability = InDesirability;
	}
	Desirability = InDesirability;
}

void URangeAIAbilityTarget::Init(AActor* InAbilityOwner, ATurnBasedBattle* InContextBattle, UBlackboardComponent* InBlackboard)
{
	Super::Init(InAbilityOwner, InContextBattle, InBlackboard);

	static const FName EnvQueryName = "EnvQuery";
	InBlackboard->SetValueAsObject(EnvQueryName, EQSLineOfSight);
	bAsync = true;
}

void URangeAIAbilityTarget::CalculateDesirabilityAsync()
{
	auto Stats = AbilityOwner->FindComponentByClass<UStatsComponent>();
	if (ensure(Stats))
	{
		if (Stats->Get(SN_ActionPoints) < Ability->GetAPCost())
		{
			OnDesirabilityFound.Broadcast(Ability, 0.f);
			return;
		}
	}

	BattleParticipants = ContextBattle->GetParticipantsWithAttitudeTo(AbilityOwner, ETeamAttitude::Hostile);
	BattleParticipantIndex = 0;

	RunLOSQuery();
}

bool URangeAIAbilityTarget::RunLOSQuery()
{
	if (ensureMsgf(EQSLineOfSight, TEXT("Line of Sight environmental query is not valid in %s"), *GetName()))
	{
		FEnvQueryRequest QueryRequest(EQSLineOfSight, AbilityOwner);

		TargetActor = BattleParticipants[BattleParticipantIndex]->GetActor();
		Blackboard->SetValueAsObject(TargetActorName, TargetActor);

		QueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &URangeAIAbilityTarget::OnEQSLineOfSightFinished);
		return true;
	}
	return false;
}

void URangeAIAbilityTarget::ChooseTargetActor()
{
	if (FRangeAttackLocations::HighestDesirability <= 0.0)
	{
		OnDesirabilityFound.Broadcast(Ability, 0.0);
	}

	TArray<AActor*> Enemies;
	RangeAttackLocations.GenerateKeyArray(Enemies);

	Enemies.FilterByPredicate(
		[this](const AActor* Key)
		{
			FRangeAttackLocations Location = *RangeAttackLocations.Find(Key);
			return Location.GetDesirability() == FRangeAttackLocations::HighestDesirability;
		});

	double ClosestOrFarthestDistance =
		PreferredDistance == EPreferredDistance::Closest ? TNumericLimits<double>::Max() : TNumericLimits<double>::Lowest();

	AActor* ChosenTarget = nullptr;
	for (AActor* Enemy : Enemies)
	{
		FRangeAttackLocations Location = *RangeAttackLocations.Find(Enemy);
		if (PreferredDistance == EPreferredDistance::Closest)
		{
			if (Location.PathToEnemyLength < ClosestOrFarthestDistance)
			{
				ClosestOrFarthestDistance = Location.PathToEnemyLength;
				ChosenTarget = Enemy;
			}
		}
		else
		{
			if (Location.PathToEnemyLength > ClosestOrFarthestDistance)
			{
				ClosestOrFarthestDistance = Location.PathToEnemyLength;
				ChosenTarget = Enemy;
			}
		}
	}

	static const FName TargetLocationName = "TargetLocation";
	FVector LOSLocation = RangeAttackLocations[ChosenTarget].LineOfSightLocation;

	Target.Actor = ChosenTarget;
	Target.Location = TargetActor->GetActorLocation();
	Blackboard->SetValueAsObject(TargetActorName, TargetActor);
	Blackboard->SetValueAsVector(TargetLocationName, LOSLocation);
}

void URangeAIAbilityTarget::OnAllPathsFound()
{
	FindDesirabilityWithAPCost();

	ChooseTargetActor();

	OnDesirabilityFound.Broadcast(Ability, RangeAttackLocations[Target.Actor].GetDesirability());
}

void URangeAIAbilityTarget::FindDesirabilityWithAPCost()
{
	TArray<AActor*> Enemies;
	RangeAttackLocations.GenerateKeyArray(Enemies);

	for (AActor* Enemy : Enemies)
	{
		FRangeAttackLocations Location = *RangeAttackLocations.Find(Enemy);
		if (!Location.bIsValid)
		{
			Location.SetDesirability(0.0);
			RangeAttackLocations.Add(Enemy, Location);
			continue;
		}

		int32 MoveAPCost;
		int32 AbilityAPCost;

		if (auto Character = Cast<ARPGCharacter>(AbilityOwner))
		{
			MoveAPCost = Character->CalculatePathAPCost(Location.PathToEnemyLength);
			AbilityAPCost = Ability->GetAPCost();

			if (Character->Stats()->Get(SN_ActionPoints) < MoveAPCost + AbilityAPCost)
			{
				Location.SetDesirability(0.0);
				RangeAttackLocations.Add(Enemy, Location);
				continue;
			}
			Location.SetDesirability(DefaultDesirability - MoveAPCost * APCostInDesirability);
			RangeAttackLocations.Add(Enemy, Location);
		}
		else
		{
			UE_LOG(LogRangeAIAbilityTarget, Error, TEXT("Non-RPGCharacter trying to use RangeAIAbilityTarget: %s"), *AbilityOwner->GetName())
			break;
		}
	}
}

void URangeAIAbilityTarget::PathToLOSFound(uint32 QueryID, ENavigationQueryResult::Type Result,
										   TSharedPtr<FNavigationPath, ESPMode::ThreadSafe> NavigationPath, AActor* Enemy)
{
	FRangeAttackLocations Locations = *RangeAttackLocations.Find(Enemy);
	if (Result == ENavigationQueryResult::Success)
	{
		Locations.PathToLOSCost = NavigationPath->GetCost();
		Locations.PathToLOSLength = NavigationPath->GetLength();
	}
	else
	{
		Locations.bIsValid = false;
	}
	RangeAttackLocations.Add(Enemy, Locations);

	LOSPathCount++;
	if (LOSPathCount >= RangeAttackLocations.Num() && EnemyPathCount >= RangeAttackLocations.Num())
	{
		OnAllPathsFound();
	}
}

void URangeAIAbilityTarget::PathToEnemyFound(uint32 QueryID, ENavigationQueryResult::Type Result,
											 TSharedPtr<FNavigationPath, ESPMode::ThreadSafe> NavigationPath, AActor* Enemy)
{
	FRangeAttackLocations Locations = *RangeAttackLocations.Find(Enemy);
	if (Result == ENavigationQueryResult::Success)
	{
		Locations.PathToEnemyCost = NavigationPath->GetCost();
		Locations.PathToEnemyLength = NavigationPath->GetLength();
	}
	else
	{
		Locations.bIsValid = false;
	}
	RangeAttackLocations.Add(Enemy, Locations);

	EnemyPathCount++;
	if (LOSPathCount >= RangeAttackLocations.Num() && EnemyPathCount >= RangeAttackLocations.Num())
	{
		OnAllPathsFound();
	}
}

void URangeAIAbilityTarget::FindPathAsync(FVector PathStart, FVector PathEnd, FNavPathQueryDelegate PathFoundQueryDelegate)
{
	FPathFindingQuery PathFindingQuery;
	PathFindingQuery.EndLocation = PathEnd;
	PathFindingQuery.StartLocation = PathStart;
	PathFindingQuery.QueryFilter = UNavigationQueryFilter::GetQueryFilter<UNavigationQueryFilter>(NavSystem->GetMainNavDataChecked());
	PathFindingQuery.NavData = &NavSystem->GetMainNavDataChecked();

	NavSystem->FindPathAsync(FNavAgentProperties::DefaultProperties, PathFindingQuery, PathFoundQueryDelegate);
}

void URangeAIAbilityTarget::AllEQSFinished()
{
	TArray<AActor*> Enemies;
	RangeAttackLocations.GenerateKeyArray(Enemies);
	LOSPathCount = 0;
	EnemyPathCount = 0;
	bool bValidLocations = false;

	for (AActor* Enemy : Enemies)
	{
		FRangeAttackLocations Locations = *RangeAttackLocations.Find(Enemy);
		if (Locations.bIsValid)
		{
			bValidLocations = true;
			FVector PathStart = AbilityOwner->GetActorLocation();
			FVector PathEnd = Locations.LineOfSightLocation;

			FNavPathQueryDelegate PathToLOSQueryDelegate;
			PathToLOSQueryDelegate.BindUObject(this, &URangeAIAbilityTarget::PathToLOSFound, Enemy);

			FindPathAsync(PathStart, PathEnd, PathToLOSQueryDelegate);

			PathStart = AbilityOwner->GetActorLocation();
			PathEnd = Locations.EnemyLocation;

			FNavPathQueryDelegate PathToEnemyQueryDelegate;
			PathToEnemyQueryDelegate.BindUObject(this, &URangeAIAbilityTarget::PathToEnemyFound, Enemy);

			FindPathAsync(PathStart, PathEnd, PathToEnemyQueryDelegate);
		}
	}

	if (!bValidLocations)
	{
		OnDesirabilityFound.Broadcast(Ability, 0.f);
	}
}

void URangeAIAbilityTarget::OnEQSLineOfSightFinished(TSharedPtr<FEnvQueryResult, ESPMode::ThreadSafe> Result)
{
	if (Result->IsSuccessful())
	{
		LineOfSightLocation = Result->GetItemAsLocation(0);

		FRangeAttackLocations Locations;
		Locations.EnemyLocation = TargetActor->GetActorLocation();
		Locations.LineOfSightLocation = LineOfSightLocation;
		RangeAttackLocations.Add(TargetActor, Locations);
	}
	else
	{
		RangeAttackLocations.Add(TargetActor, FRangeAttackLocations::Invalid);
	}

	BattleParticipantIndex++;
	if (BattleParticipantIndex >= BattleParticipants.Num())
	{
		AllEQSFinished();
	}
	else
	{
		RunLOSQuery();
	}
}
