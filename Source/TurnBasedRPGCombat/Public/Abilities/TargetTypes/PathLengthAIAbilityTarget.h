// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/AIAbilityTarget.h"
#include "PathLengthAIAbilityTarget.generated.h"

/**
 * Chooses a target based on a path distance
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UPathLengthAIAbilityTarget : public UAIAbilityTarget
{
	GENERATED_BODY()
public:
	UPathLengthAIAbilityTarget();
	virtual void Init(AActor* InAbilityOwner, ATurnBasedBattle* InContextBattle, UBlackboardComponent* InBlackboard) override;
	virtual float CalculateDesirability() override;
	virtual void CalculateDesirabilityAsync() override;

private:
	UPROPERTY(EditAnywhere)
	float MinDistance;

	TArray<AActor*> GetAllEnemiesInBattle();
	TArray<float> FindWeights();
	void DoneFindingWeights();
	void FindWeightForNextActor();
	float FindHighestWeight();

	UPROPERTY()
	TArray<float> Weights;
	UPROPERTY()
	TArray<AActor*> Enemies;
	int32 EnemyIndex;
	bool bFindPath = false;
	UPROPERTY()
	AActor* TargetEnemy;

	void PathFound(uint32 QueryID, ENavigationQueryResult::Type Result, TSharedPtr<FNavigationPath, ESPMode::ThreadSafe> NavigationPath);
	void FindPathToActorAsync(ANavigationData* NavData);
	UFUNCTION()
	void OnNavigationRebuild(ANavigationData* NavData);
};
