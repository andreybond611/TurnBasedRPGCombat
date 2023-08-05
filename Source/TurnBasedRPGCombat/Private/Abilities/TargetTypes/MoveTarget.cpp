// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/MoveTarget.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/TurnBasedUtility.h"
#include "NavigationPath.h"
#include "Abilities/Ability.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Components/DecalComponent.h"
#include "UnrealFramework/RPGPlayerController.h"

UMoveTarget::UMoveTarget()
{
	bEnableRotating = false;
}

void UMoveTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);

	NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
}

void UMoveTarget::AddTargetInfoActor(UNavigationPath* DestinationPath, AActor* Actor)
{
	if (!DestinationPath)
	{
		PlayerController->AddTargetInfoSectionCantReachDestination();
		return;
	}

	float Distance = FVector::Distance(Actor->GetActorLocation(), ControlledCharacter->GetLocation());
	if (!DestinationPath->IsValid() && Distance > AcceptanceDistance)
	{
		PlayerController->AddTargetInfoSectionCantReachDestination();
		return;
	}

	// todo: return the code below

	// float Accuracy = ControlledCharacter->CalculateHitChance(Actor);
	// PlayerController->AddTargetInfoSectionAccuracy(Accuracy);

	// int32 MoveAPCost = ControlledCharacter->CalculatePathAPCost(DestinationPath);
	// int32 ActionAPCost = Ability->GetAPCost();
	// PlayerController->AddTargetInfoSectionAPCost(MoveAPCost + ActionAPCost);

	// if (MoveAPCost + ActionAPCost > ControlledCharacter->Stats()->Get(SActionPoints))
	//{
	//	FText Message = NSLOCTEXT("UI", "NotEnoughAP", "Not enough AP");
	//	PlayerController->AddTargetInfoSection(Message, 0, FColor::Red);
	// }
}

void UMoveTarget::ShowDestination(const FHitResult& HitResult)
{
	UNavigationPath* DestinationPath = nullptr;
	AActor* HitActor = HitResult.GetActor();
	Target.Location = HitResult.Location;
	if (HitActor->GetClass()->ImplementsInterface(UTBBattleParticipant::StaticClass()))
	{
		if (IsDetected(HitActor))
		{
			DestinationPath = FindPathToActor(HitActor);

			Target.Actor = HitActor;

			AddTargetInfoActor(DestinationPath, HitActor);
		}
	}
	else
	{
		DestinationPath = FindPathToLocation(HitResult.Location);

		AddTargetInfoLocation(DestinationPath);
	}
	SpawnDecalsOnPath(DestinationPath);
}

void UMoveTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	RemoveDestination();

	if (bShowPath)
	{
		ShowDestination(CursorHitResult);
	}
}

void UMoveTarget::StopTargeting()
{
	EnableDynamicObstacle();
	RemoveDestination();
}

void UMoveTarget::NavigationGenerationFinished(ANavigationData* NavData)
{
	bShowPath = true;
	bAllowMove = true;

	NavSystem->OnNavigationGenerationFinishedDelegate.RemoveDynamic(this, &UMoveTarget::NavigationGenerationFinished);
}

void UMoveTarget::SpawnDecalsOnPath(UNavigationPath* NavigationPath)
{
	if (NavigationPath && NavigationPath->IsValid() && bShowPath)
	{
		TArray<FVector> FoundPoints = UTurnBasedUtility::FindPointsAlongPath(NavigationPath, DistanceBetweenPathDecals);

		for (FVector Point : FoundPoints)
		{
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(this, PointDecalMaterial, PointDecalSize, Point);
			PathDecals.Add(Decal);
		}

		if (!NavigationPath->PathPoints.IsEmpty())
		{
			FVector DestinationPoint = NavigationPath->PathPoints.Last();
			if (!DestinationDecalComponent)
			{
				DestinationDecalComponent =
					UGameplayStatics::SpawnDecalAtLocation(this, LastPointDecalMaterial, PointDecalSize * 5.f, DestinationPoint);
			}
		}
	}
}

UNavigationPath* UMoveTarget::FindPathToActor(AActor* Actor)
{
	FVector PathStart = ControlledCharacter->GetLocation();

	// todo: eliminate additional pathfind and work out a better way to find closest location that doesn't rely on affecting navigation capsule
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, PathStart, Actor);

	if (!NavigationPath)
	{
		return nullptr;
	}

	DisableDynamicObstacle(Actor);

	if (NavigationPath->GetPathLength() < AcceptanceDistance)
	{
		return NavigationPath;
	}

	FVector SearchedLocation = UTurnBasedUtility::GetLocationOnPathNearActor(NavigationPath, AcceptanceDistance);
	return UNavigationSystemV1::FindPathToLocationSynchronously(this, PathStart, SearchedLocation);
}

UNavigationPath* UMoveTarget::FindPathToLocation(const FVector& Location)
{
	EnableDynamicObstacle();

	FVector PathStart = ControlledCharacter->GetLocation();
	FVector PathEnd = Location;
	return UNavigationSystemV1::FindPathToLocationSynchronously(this, PathStart, PathEnd);
}

void UMoveTarget::AddTargetInfoLocation(UNavigationPath* DestinationPath)
{
	if (!DestinationPath)
	{
		return;
	}

	if (DestinationPath && !DestinationPath->IsValid())
	{
		PlayerController->AddTargetInfoSectionCantReachDestination();
	}

	float PathDistance = DestinationPath->GetPathLength();
	int32 MoveAPCost = ControlledCharacter->CalculatePathAPCost(DestinationPath);
	PlayerController->AddTargetInfoSectionAPCost(MoveAPCost);

	PlayerController->AddTargetInfoSectionDistance(PathDistance);

	if (MoveAPCost > ControlledCharacter->Stats()->Get(SN_ActionPoints))
	{
		FText Message = NSLOCTEXT("UI", "NotEnoughAPToReachDestination", "Not enough AP to reach destination!");
		PlayerController->AddTargetInfoSection(Message, 0, FColor::Yellow);
	}
}

void UMoveTarget::RemoveDecals()
{
	for (UDecalComponent* Dot : PathDecals)
	{
		Dot->DestroyComponent();
	}
	PathDecals.Empty();

	if (DestinationDecalComponent)
	{
		DestinationDecalComponent->DestroyComponent();
		DestinationDecalComponent = nullptr;
	}
}

void UMoveTarget::RemoveDestination()
{
	RemoveDecals();
	Target.Actor = nullptr;
	PlayerController->ResetTargetInfo();
}

void UMoveTarget::DisableDynamicObstacle(AActor* Actor)
{
	if (!DynamicObstacle || DynamicObstacle != Actor)
	{
		auto DynamicObstacleInterface = Cast<IDynamicObstacle>(Actor);
		if (DynamicObstacleInterface)
		{
			bShowPath = false;
			bAllowMove = false;

			if (DynamicObstacle && DynamicObstacle != DynamicObstacleInterface)
			{
				DynamicObstacle->EnableDynamicObstacle();
			}

			DynamicObstacle.SetInterface(DynamicObstacleInterface);
			DynamicObstacle.SetObject(Actor);
			DynamicObstacle->DisableDynamicObstacle();

			NavSystem->OnNavigationGenerationFinishedDelegate.AddDynamic(this, &UMoveTarget::NavigationGenerationFinished);
		}
	}
}

void UMoveTarget::EnableDynamicObstacle()
{
	if (DynamicObstacle)
	{
		bShowPath = false;
		bAllowMove = false;

		DynamicObstacle->EnableDynamicObstacle();
		DynamicObstacle = nullptr;

		NavSystem->OnNavigationGenerationFinishedDelegate.AddDynamic(this, &UMoveTarget::NavigationGenerationFinished);
	}
}
