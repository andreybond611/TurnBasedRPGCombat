// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Utility/TurnBasedUtility.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

FVector UTurnBasedUtility::GetLocationOnPathNearActor(UNavigationPath* PathToActor, float DistanceToActor)
{
	TArray<FNavPathPoint> PathPoints = PathToActor->GetPath()->GetPathPoints();
	TArray<FLineSegment> PathSegments = GetLineSegmentsFromPathPoints(PathPoints, true);
	return GetLocationWithDistanceAlongPathSegments(PathSegments, DistanceToActor);
}

TArray<FLineSegment> UTurnBasedUtility::GetLineSegmentsFromPathPoints(const TArray<FNavPathPoint>& InPathPoints, bool bReversed)
{
	TArray<FLineSegment> PathSegments;
	FNavPathPoint PreviousPoint = FVector::Zero();

	if (bReversed)
	{
		for (int32 PathPointCount = InPathPoints.Num() - 1; PathPointCount >= 0; --PathPointCount)
		{
			if (PreviousPoint == FVector::Zero())
			{
				PreviousPoint = InPathPoints[PathPointCount];
				continue;
			}
			PathSegments.Add({PreviousPoint.Location, InPathPoints[PathPointCount].Location});
			PreviousPoint = InPathPoints[PathPointCount];
		}
	}
	else
	{
		for (int32 PathPointCount = 0; PathPointCount < InPathPoints.Num(); ++PathPointCount)
		{
			if (PreviousPoint == FVector::Zero())
			{
				PreviousPoint = InPathPoints[PathPointCount];
				continue;
			}
			PathSegments.Add({PreviousPoint.Location, InPathPoints[PathPointCount].Location});
			PreviousPoint = InPathPoints[PathPointCount];
		}
	}

	return PathSegments;
}

FVector UTurnBasedUtility::GetLocationWithDistanceAlongPathSegments(const TArray<FLineSegment>& PathSegments, float Distance)
{
	FVector SearchedLocation;
	float SearchedDistance = Distance;
	for (FLineSegment Segment : PathSegments)
	{
		if (Segment.Vector().Length() > SearchedDistance)
		{
			FVector NormalizedSegment = Segment.Vector().GetSafeNormal();
			SearchedLocation = NormalizedSegment * SearchedDistance + Segment.LineStart;
			break;
		}
		SearchedDistance -= Segment.Vector().Length();
	}

	return SearchedLocation;
}

EOutlineColor UTurnBasedUtility::AttitudeToOutlineColor(ETeamAttitude::Type Attitude)
{
	switch (Attitude)
	{
	case ETeamAttitude::Friendly: return EOutlineColor::Green;
	case ETeamAttitude::Neutral: return EOutlineColor::Yellow;
	case ETeamAttitude::Hostile: return EOutlineColor::Red;
	default: return EOutlineColor::White;
	}
}

EHitDirection UTurnBasedUtility::FindHitDirection(AActor* Attacker, AActor* Target)
{
	return FindHitDirection(Attacker->GetActorLocation(), Target);
}

EHitDirection UTurnBasedUtility::FindHitDirection(FVector AttackLocation, AActor* Target)
{
	AttackLocation.Z = 0;
	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z = 0;

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, AttackLocation);
	FRotator TargetRotation = Target->GetActorRotation();
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(LookAtRotation, TargetRotation);

	if (DeltaRotation.Yaw > -45.f && DeltaRotation.Yaw < 45.f)
	{
		return EHitDirection::Front;
	}
	if (DeltaRotation.Yaw < 135.f && DeltaRotation.Yaw >= 45.f)
	{
		return EHitDirection::Right;
	}
	if (DeltaRotation.Yaw > -135.f && DeltaRotation.Yaw <= -45.f)
	{
		return EHitDirection::Back;
	}
	return EHitDirection::Left;
}

//todo: refactor it somehow idk
TArray<FVector> UTurnBasedUtility::FindPointsAlongPath(UNavigationPath* NavigationPath, float DistanceBetweenPoints)
{
	TArray<FVector> Result;
	float SumOfLengths = 0.f;

	FVector PreviousPoint = FVector::ZeroVector;
	for (FVector Point : NavigationPath->PathPoints)
	{
		if (PreviousPoint == FVector::ZeroVector)
		{
			PreviousPoint = Point;
			Result.Add(Point);
			continue;
		}

		FVector VectorToNextPathPoint = Point - PreviousPoint;
		float ToNextPathPointLength = VectorToNextPathPoint.Length();
		SumOfLengths += ToNextPathPointLength;

		while (SumOfLengths >= DistanceBetweenPoints)
		{
			FVector Normal = VectorToNextPathPoint.GetSafeNormal();
			float DistanceToNextPointAlongPath = DistanceBetweenPoints - (SumOfLengths - ToNextPathPointLength);
			FVector PointAlongPath = Normal * DistanceToNextPointAlongPath + PreviousPoint;
			SumOfLengths -= DistanceBetweenPoints;
			Result.Add(PointAlongPath);
			if (SumOfLengths >= DistanceBetweenPoints)
			{
				PreviousPoint = PointAlongPath;
				ToNextPathPointLength -= DistanceToNextPointAlongPath;
			}
		}
		PreviousPoint = Point;
	}

	return Result;
}
