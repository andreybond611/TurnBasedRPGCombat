// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/ArcTarget.h"

#include "Abilities/TargetTypes/Helpers/ProjectileSplineActor.h"
#include "Characters/RPGCharacter.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/SplineMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealFramework/RPGPlayerController.h"
#include "UnrealFramework/RPGPlayerState.h"

void UArcTarget::StartTargeting(ARPGPlayerController* InPlayerController)
{
	Super::StartTargeting(InPlayerController);
}

bool UArcTarget::FindSuggestedVelocity(const FHitResult& HitResult, FVector& OutTossVelocity)
{
	FCollisionResponseParams Response = FCollisionResponseParams::DefaultResponseParam;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(ControlledCharacter);
	return UGameplayStatics::SuggestProjectileVelocity(this, OutTossVelocity, StartLocation, HitResult.Location, LaunchSpeed, false, ProjectileRadius,
													   0.f, ESuggestProjVelocityTraceOption::DoNotTrace,
													   FCollisionResponseParams::DefaultResponseParam, ActorsToIgnore, false);
}

bool UArcTarget::PredictProjectilePath(FVector InTossVelocity, FPredictProjectilePathResult& OutResult)
{
	float MaxSimTime = 2.f;
	FPredictProjectilePathParams Params{ProjectileRadius, StartLocation, InTossVelocity, MaxSimTime};

	TArray<ARPGCharacter*> PlayerParty;
	auto PlayerState = PlayerController->GetPlayerState<ARPGPlayerState>();
	if (PlayerState)
	{
		PlayerParty = PlayerState->GetPlayerParty();
	}

	Params.bTraceWithCollision = true;
	Params.bTraceComplex = true;
	Params.ActorsToIgnore.Append(PlayerParty);
	Params.DrawDebugType = EDrawDebugTrace::None;
	Params.bTraceWithChannel = true;
	Params.SimFrequency = 80.f;
	Params.TraceChannel = ECollisionChannel::ECC_Camera;

	FPredictProjectilePathResult Result;
	bool bHit = UGameplayStatics::PredictProjectilePath(this, Params, Result);

	OutResult = Result;
	return bHit;
}

void UArcTarget::AddMeshToLocation(FVector EndLocation)
{
	UStaticMeshComponent* SplineEndMeshComponent =
		Cast<UStaticMeshComponent>(SplineActor->AddComponentByClass(UStaticMeshComponent::StaticClass(), true, {}, false));

	if (SplineEndMeshComponent && SplineEndMesh)
	{
		SplineEndMeshComponent->SetRelativeLocation(EndLocation);
		SplineEndMeshComponent->SetStaticMesh(SplineEndMesh);
		SplineEndMeshComponent->SetMaterial(0, SplineEndMeshMaterial);
		SplineEndMeshComponent->SetRelativeScale3D(FVector(SplineEndScale));
		SplineEndMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UArcTarget::CreateSplineMesh(USplineComponent* SplineComponent)
{
	int32 SplineIndex;
	for (SplineIndex = 0; SplineIndex < SplineComponent->GetNumberOfSplinePoints() - 1; ++SplineIndex)
	{
		USplineMeshComponent* SplineMeshComponent = SplineActor->AddSplineMeshComponent();
		SplineMeshComponent->SetStartScale(FVector2D{MeshScale});
		SplineMeshComponent->SetEndScale(FVector2D{MeshScale});
		SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
		SplineMeshComponent->SetMaterial(0, ProjectilePathMaterial);
		SplineMeshComponent->SetStaticMesh(ProjectilePathMesh);

		FVector StartPosition = SplineComponent->GetLocationAtSplinePoint(SplineIndex, ESplineCoordinateSpace::Local);
		FVector EndPosition = SplineComponent->GetLocationAtSplinePoint(SplineIndex + 1, ESplineCoordinateSpace::Local);

		FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineIndex, ESplineCoordinateSpace::Local);
		FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineIndex + 1, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
	}

	FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(SplineIndex, ESplineCoordinateSpace::World);
	AddMeshToLocation(EndLocation);
}

void UArcTarget::CreateArcSpline(FVector InTossVelocity, FPredictProjectilePathResult Result)
{
	FRotator SplineRotation = InTossVelocity.Rotation();
	SplineActor = GetWorld()->SpawnActor<AProjectileSplineActor>(*ProjectileSplineActorClass, StartLocation, SplineRotation);
	USplineComponent* SplineComponent = SplineActor->GetSplineComponent();

	SplineComponent->ClearSplinePoints();

	int32 SplineIndex = 0;
	for (auto PathPoint : Result.PathData)
	{
		SplineComponent->AddSplinePoint(PathPoint.Location, ESplineCoordinateSpace::World);
		SplineIndex++;
	}
	SplineComponent->SetSplinePointType(SplineIndex, ESplinePointType::CurveClamped);

	CreateSplineMesh(SplineComponent);
}

void UArcTarget::RemoveSpline()
{
	if (SplineActor)
	{
		SplineActor->Destroy();
	}
}

void UArcTarget::TickTargetAbility(const FHitResult& CursorHitResult)
{
	RemoveSpline();

	USceneComponent* ProjectileStart = ControlledCharacter->GetProjectileStartComponent();
	StartLocation = ProjectileStart->GetComponentLocation();

	bSuggestedVelocityFound = FindSuggestedVelocity(CursorHitResult, TossVelocity);

	if (bSuggestedVelocityFound)
	{
		bool bHit = PredictProjectilePath(TossVelocity, PredictPathResult);

		if (bHit)
		{
			CreateArcSpline(TossVelocity, PredictPathResult);
		}
	}
}

void UArcTarget::StopTargeting()
{
	RemoveSpline();
}
