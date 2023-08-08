// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "TurnBasedTypes.h"
#include "Characters/RPGCharacter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TurnBasedUtility.generated.h"

class UNavigationPath;
/**
 * 
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UTurnBasedUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FVector GetLocationOnPathNearActor(UNavigationPath* PathToActor, float DistanceToActor);

	static TArray<FLineSegment> GetLineSegmentsFromPathPoints(const TArray<FNavPathPoint>& InPathPoints, bool bReversed = false);
	static FVector GetLocationWithDistanceAlongPathSegments(const TArray<FLineSegment>& PathSegments, float Distance);

	static EOutlineColor AttitudeToOutlineColor(ETeamAttitude::Type Attitude);

	static EHitDirection FindHitDirection(AActor* Attacker, AActor* Target);
	static EHitDirection FindHitDirection(FVector AttackLocation, AActor* Target);
	static TArray<FVector> FindPointsAlongPath(UNavigationPath* NavigationPath, float DistanceBetweenPoints);

	static FName DamageTypeToResistanceStat(EDamageType DamageType);
};
