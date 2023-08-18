// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Utility/TurnBasedUtility.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/GenericPlatform.h"

FVector UTurnBasedUtility::GetLocationOnPathNearActor(UNavigationPath* PathToActor, const float DistanceToActor)
{
	const TArray<FNavPathPoint> PathPoints = PathToActor->GetPath()->GetPathPoints();
	const TArray<FLineSegment> PathSegments = GetLineSegmentsFromPathPoints(PathPoints, true);
	return GetLocationWithDistanceAlongPathSegments(PathSegments, DistanceToActor);
}

TArray<FLineSegment> UTurnBasedUtility::GetLineSegmentsFromPathPoints(const TArray<FNavPathPoint>& InPathPoints, const bool bReversed)
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

FVector UTurnBasedUtility::GetLocationWithDistanceAlongPathSegments(const TArray<FLineSegment>& PathSegments, const float Distance)
{
	FVector SearchedLocation;
	float SearchedDistance = Distance;
	for (FLineSegment Segment : PathSegments)
	{
		if (Segment.Vector().Length() > SearchedDistance)
		{
			const FVector NormalizedSegment = Segment.Vector().GetSafeNormal();
			SearchedLocation = NormalizedSegment * SearchedDistance + Segment.LineStart;
			break;
		}
		SearchedDistance -= Segment.Vector().Length();
	}

	return SearchedLocation;
}

EOutlineColor UTurnBasedUtility::AttitudeToOutlineColor(const ETeamAttitude::Type Attitude)
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

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, AttackLocation);
	const FRotator TargetRotation = Target->GetActorRotation();
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(LookAtRotation, TargetRotation);

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

// todo: refactor it somehow idk
TArray<FVector> UTurnBasedUtility::FindPointsAlongPath(UNavigationPath* NavigationPath, const float DistanceBetweenPoints)
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
			const float DistanceToNextPointAlongPath = DistanceBetweenPoints - (SumOfLengths - ToNextPathPointLength);
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

FName UTurnBasedUtility::DamageTypeToResistanceStat(const EDamageType DamageType)
{
	switch (DamageType)
	{
	case EDamageType::Physical: return SN_PhysicalArmor;
	case EDamageType::Fire: return SN_FireResistance;
	case EDamageType::Earth: return SN_EarthResistance;
	case EDamageType::Poison: return SN_PoisonResistance;
	case EDamageType::Ice: return SN_IceResistance;
	default: checkNoEntry(); return NAME_None;
	}
}

FString UTurnBasedUtility::ObjectFlagToString(const EObjectFlags ObjectFlags)
{
	static TArray<FString> FlagNames = {
		"RF_NoFlags", ///< No flags, used to avoid a cast

		// This first group of flags mostly has to do with what kind of object it is. Other than transient, these are the persistent object flags.
		// The garbage collector also tends to look at these.
		"RF_Public",			 ///< Object is visible outside its package.
		"RF_Standalone",		 ///< Keep object around for editing even if unreferenced.
		"RF_MarkAsNative",		 ///< Object (UField) will be marked as native on construction (DO NOT USE THIS FLAG in HasAnyFlags() etc)
		"RF_Transactional",		 ///< Object is transactional.
		"RF_ClassDefaultObject", ///< This object is its class's default object
		"RF_ArchetypeObject",	 ///< This object is a template for another object - treat like a class default object
		"RF_Transient",			 ///< Don't save object.

		// This group of flags is primarily concerned with garbage collection.
		"RF_MarkAsRootSet", ///< Object will be marked as root set on construction and not be garbage collected, even if unreferenced (DO NOT USE THIS
							///< FLAG in HasAnyFlags() etc)
		"RF_TagGarbageTemp", ///< This is a temp user flag for various utilities that need to use the garbage collector. The garbage collector itself
							 ///< does not interpret it.

		// The group of flags tracks the stages of the lifetime of a uobject
		"RF_NeedInitialization",	 ///< This object has not completed its initialization process. Cleared when ~FObjectInitializer completes
		"RF_NeedLoad",				 ///< During load, indicates object needs loading.
		"RF_KeepForCooker",			 ///< Keep this object during garbage collection because it's still being used by the cooker
		"RF_NeedPostLoad",			 ///< Object needs to be postloaded.
		"RF_NeedPostLoadSubobjects", ///< During load, indicates that the object still needs to instance subobjects and fixup serialized component
									 ///< references
		"RF_NewerVersionExists",	 ///< Object has been consigned to oblivion due to its owner package being reloaded, and a newer version currently
									 ///< exists
		"RF_BeginDestroyed",		 ///< BeginDestroy has been called on the object.
		"RF_FinishDestroyed",		 ///< FinishDestroy has been called on the object.

		// Misc. Flags
		"RF_BeingRegenerated", ///< Flagged on UObjects that are used to create UClasses (e.g. Blueprints) while they are regenerating their UClass on
							   ///< load (See FLinkerLoad::CreateExport()), as well as UClass objects in the midst of being created
		"RF_DefaultSubObject", ///< Flagged on subobjects that are defaults
		"RF_WasLoaded",		   ///< Flagged on UObjects that were loaded
		"RF_TextExportTransient", ///< Do not export object to text form (e.g. copy/paste). Generally used for sub-objects that can be regenerated
								  ///< from data in their parent object.
		"RF_LoadCompleted", ///< Object has been completely serialized by linkerload at least once. DO NOT USE THIS FLAG, It should be replaced with
							///< RF_WasLoaded.
		"RF_InheritableComponentTemplate", ///< Archetype of the object can be in its super class
		"RF_DuplicateTransient",		   ///< Object should not be included in any type of duplication (copy/paste, binary duplication, etc.)
		"RF_StrongRefOnFrame",			   ///< References to this object from persistent function frame are handled as strong ones.
		"RF_NonPIEDuplicateTransient",	   ///< Object should not be included for duplication unless it's being duplicated for a PIE session
		"RF_Dynamic", ///< Field Only. Dynamic field - doesn't get constructed during static initialization, can be constructed multiple times  //
		"RF_WillBeLoaded",		 ///< This object was constructed during load and will be loaded shortly
		"RF_HasExternalPackage", ///< This object has an external package assigned and should look it up when getting the outermost package

		// RF_Garbage and RF_PendingKill are mirrored in EInternalObjectFlags because checking the internal flags is much faster for the Garbage
		// Collector while checking the object flags is much faster outside of it where the Object pointer is already available and most likely
		// cached. RF_PendingKill is mirrored in EInternalObjectFlags because checking the internal flags is much faster for the Garbage Collector
		// while checking the object flags is much faster outside of it where the Object pointer is already available and most likely cached.

		"RF_PendingKill", ///< Objects that are pending destruction (invalid for gameplay but valid objects). This flag is mirrored in
						  ///< EInternalObjectFlags as PendingKill for performance
		"RF_Garbage", ///< Garbage from logical point of view and should not be referenced. This flag is mirrored in EInternalObjectFlags as Garbage
					  ///< for performance
		"RF_AllocatedInSharedPage",
	};

	FString FlagsToPrint;

	int64 Flag = 1;
	for (int32 StringIndex = 1; Flag != 0x80000000; ++StringIndex, Flag = Flag << 1)
	{
		if (ObjectFlags & Flag)
		{
			FlagsToPrint.Append(" " + FlagNames[StringIndex]);
		}
	}

	return FlagsToPrint;
}
