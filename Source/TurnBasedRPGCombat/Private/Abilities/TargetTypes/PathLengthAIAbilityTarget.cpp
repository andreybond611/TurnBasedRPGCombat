// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/PathLengthAIAbilityTarget.h"

#include "GenericTeamAgentInterface.h"
#include "NavigationSystem.h"
#include "AI/Pathfinding/DynamicObstacle.h"
#include "TurnBased/TurnBasedBattle.h"
#include "TurnBased/TurnBasedBattleParticipant.h"

TArray<AActor*> UPathLengthAIAbilityTarget::GetAllEnemiesInBattle()
{
	TArray<AActor*> Result;
	for (TScriptInterface<ITBBattleParticipant> BattleParticipant : ContextBattle->GetParticipants())
	{
		if (auto* TeamAgent = Cast<IGenericTeamAgentInterface>(AbilityOwner))
		{
			if (TeamAgent->GetTeamAttitudeTowards(*BattleParticipant->GetActor()) == ETeamAttitude::Hostile)
			{
				Result.Add(BattleParticipant->GetActor());
			}
		}
	}
	return Result;
}

TArray<float> UPathLengthAIAbilityTarget::FindWeights()
{
	for (AActor* Enemy : Enemies)
	{
		double PathLength;
		ENavigationQueryResult::Type Result =
			UNavigationSystemV1::GetPathLength(AbilityOwner, AbilityOwner->GetActorLocation(), Enemy->GetActorLocation(), PathLength);

		if (Result == ENavigationQueryResult::Success)
		{
			if (PathLength > MaxRange)
			{
				Weights.Add(0.f);
				continue;
			}
			// less distance == more weight
			float Weight = 1 - ((PathLength - MinDistance) / (MaxRange - MinDistance));
			Weight = FMath::Clamp(Weight, 0.f, 1.f);
			Weights.Add(Weight);
			continue;
		}
		Weights.Add(0.f);
	}
	return Weights;
}

void UPathLengthAIAbilityTarget::DoneFindingWeights()
{
	float Result = FindHighestWeight();

	NavSystem->OnNavigationGenerationFinishedDelegate.RemoveDynamic(this, &UPathLengthAIAbilityTarget::OnNavigationRebuild);
	OnDesirabilityFound.Broadcast(Ability, Result);
}

void UPathLengthAIAbilityTarget::FindWeightForNextActor()
{
	if (!Enemies.IsValidIndex(EnemyIndex))
	{
		DoneFindingWeights();
		return;
	}

	AActor* Enemy = Enemies[EnemyIndex];
	if (auto DynamicObstacle = Cast<IDynamicObstacle>(Enemy))
	{
		bFindPath = true;
		// forces to rebuild navigation. see OnNavigationRebuild()
		DynamicObstacle->DisableDynamicObstacle();
	}
}

void UPathLengthAIAbilityTarget::PathFound(uint32 QueryID, ENavigationQueryResult::Type Result, TSharedPtr<FNavigationPath, ESPMode::ThreadSafe> NavigationPath)
{
	float Distance = NavigationPath->GetLength();
	if (Result == ENavigationQueryResult::Success)
	{
		if (Distance > MaxRange)
		{
			Weights.Insert(0.f, EnemyIndex);
		}
		else
		{
			// less distance == more weight
			float Weight = 1 - ((Distance - MinDistance) / (MaxRange - MinDistance));
			Weight = FMath::Clamp(Weight, 0.f, 1.f);
			Weights.Insert(Weight, EnemyIndex);
		}
	}
	else
	{
		Weights.Insert(0.f, EnemyIndex);
	}

	bFindPath = false;
	if (auto DynamicObstacle = Cast<IDynamicObstacle>(Enemies[EnemyIndex]))
	{
		// forces to rebuild navigation. see OnNavigationRebuild()
		DynamicObstacle->EnableDynamicObstacle();
	}
}

void UPathLengthAIAbilityTarget::FindPathToActorAsync(ANavigationData* NavData)
{
	AActor* Enemy = Enemies[EnemyIndex];

	FPathFindingQuery PathFindingQuery;
	PathFindingQuery.EndLocation = Enemy->GetActorLocation();
	PathFindingQuery.StartLocation = AbilityOwner->GetActorLocation();
	PathFindingQuery.QueryFilter = UNavigationQueryFilter::GetQueryFilter<UNavigationQueryFilter>(*NavData);
	PathFindingQuery.NavData = NavData;

	FNavPathQueryDelegate PathQueryDelegate;
	PathQueryDelegate.BindUObject(this, &UPathLengthAIAbilityTarget::PathFound);

	NavSystem->FindPathAsync(FNavAgentProperties::DefaultProperties, PathFindingQuery, PathQueryDelegate);
}

void UPathLengthAIAbilityTarget::OnNavigationRebuild(ANavigationData* NavData)
{
	if (bFindPath)
	{
		FindPathToActorAsync(NavData);
	}
	else
	{
		EnemyIndex++;
		FindWeightForNextActor();
	}
}

float UPathLengthAIAbilityTarget::FindHighestWeight()
{
	float HighestWeight = 0.f;
	int32 Index = 0;
	int32 HighestWeightIndex = 0;
	for (float Weight : Weights)
	{
		if (Weight > HighestWeight)
		{
			HighestWeight = Weight;
			HighestWeightIndex = Index;
		}
		Index++;
	}

	if (HighestWeight > 0.f)
	{
		Enemies = GetAllEnemiesInBattle();
		Target.Actor = Enemies[HighestWeightIndex];
	}

	return HighestWeight;
}

UPathLengthAIAbilityTarget::UPathLengthAIAbilityTarget()
{
	bAsync = true;
}

void UPathLengthAIAbilityTarget::Init(AActor* InAbilityOwner, ATurnBasedBattle* InContextBattle, UBlackboardComponent* InBlackboard)
{
	Super::Init(InAbilityOwner, InContextBattle, InBlackboard);

	Weights.Empty();
	Enemies.Empty();
	EnemyIndex = 0;
	NavSystem->OnNavigationGenerationFinishedDelegate.AddDynamic(this, &UPathLengthAIAbilityTarget::OnNavigationRebuild);
}

float UPathLengthAIAbilityTarget::CalculateDesirability()
{
	Enemies = GetAllEnemiesInBattle();
	if (Enemies.IsEmpty())
	{
		return 0.f;
	}

	Weights = FindWeights();

	return FindHighestWeight();
}

void UPathLengthAIAbilityTarget::CalculateDesirabilityAsync()
{
	EnemyIndex = 0;
	Enemies = GetAllEnemiesInBattle();
	Weights.Empty();
	FindWeightForNextActor();
}
