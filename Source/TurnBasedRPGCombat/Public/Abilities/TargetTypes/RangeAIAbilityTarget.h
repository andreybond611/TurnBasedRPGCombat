// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "EQSAIAbilityTarget.h"
#include "RangeAIAbilityTarget.generated.h"

class ITBBattleParticipant;
struct FEnvQueryResult;
class UBlackboardComponent;

USTRUCT()
struct FRangeAttackLocations
{
	GENERATED_BODY()

	FRangeAttackLocations() = default;
	FRangeAttackLocations(bool bInIsValid)
		: bIsValid(bInIsValid){};

	static FRangeAttackLocations Invalid;

	FVector EnemyLocation;
	FVector LineOfSightLocation;

	double PathToEnemyCost = 0.0;
	double PathToLOSCost = 0.0;

	double PathToEnemyLength = 0.0;
	double PathToLOSLength = 0.0;

	inline static double HighestDesirability = 0.0;

	void SetDesirability(double InDesirability);
	double GetDesirability() const { return Desirability; }

	bool bIsValid = true;

private:
	double Desirability = 0.0;
};

/**
 * Chooses a target for a range ability based on distance, and also runs EQS query
 * finding a better location for the ability if there are obstacles on the way
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API URangeAIAbilityTarget : public UAIAbilityTarget
{
	GENERATED_BODY()
public:
	virtual void Init(AActor* InAbilityOwner, ATurnBasedBattle* InContextBattle, UBlackboardComponent* InBlackboard) override;
	virtual void CalculateDesirabilityAsync() override;

private:
	UPROPERTY()
	TMap<float, AActor*> Distances;

	UPROPERTY(EditAnywhere)
	UEnvQuery* EQSLineOfSight;
	UPROPERTY(EditAnywhere)
	bool bCustomMaxPathLength = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bCustomMaxPathLength", EditConditionHides))
	float MaxPathLength = 100.f;
	UPROPERTY(EditAnywhere)
	float DefaultDesirability = 85.f;
	UPROPERTY(EditAnywhere)
	float APCostInDesirability = 20.f;
	UPROPERTY(EditAnywhere)
	EPreferredDistance PreferredDistance = EPreferredDistance::Closest;

	UPROPERTY()
	TArray<TScriptInterface<ITBBattleParticipant>> BattleParticipants;
	UPROPERTY()
	TMap<AActor*, FRangeAttackLocations> RangeAttackLocations;
	UPROPERTY()
	AActor* TargetActor;

	int32 BattleParticipantIndex = 0;
	bool bQuerySuccess;
	FVector LineOfSightLocation;
	int32 LOSPathCount;
	int32 EnemyPathCount;

	void FindPathAsync(FVector PathStart, FVector PathEnd, FNavPathQueryDelegate PathFoundQueryDelegate);
	void PathToLOSFound(uint32 QueryID, ENavigationQueryResult::Type Result, TSharedPtr<FNavigationPath, ESPMode::ThreadSafe> NavigationPath,
						AActor* Enemy);
	void PathToEnemyFound(uint32 QueryID, ENavigationQueryResult::Type Result, TSharedPtr<FNavigationPath, ESPMode::ThreadSafe> NavigationPath,
						  AActor* Enemy);
	void AllEQSFinished();
	void OnEQSLineOfSightFinished(TSharedPtr<FEnvQueryResult, ESPMode::ThreadSafe> Result);
	bool RunLOSQuery();
	void ChooseTargetActor();
	void OnAllPathsFound();
	void FindDesirabilityWithAPCost();

	inline static const FName TargetActorName = "TargetActor";
};
