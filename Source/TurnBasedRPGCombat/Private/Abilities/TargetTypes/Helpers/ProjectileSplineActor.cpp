// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/TargetTypes/Helpers/ProjectileSplineActor.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

AProjectileSplineActor::AProjectileSplineActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(SplineComponent);
}

USplineMeshComponent* AProjectileSplineActor::AddSplineMeshComponent()
{
	if (USplineMeshComponent* SplineMeshComponent =
			Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), true, FTransform{}, false)))
	{
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		return SplineMeshComponent;
	}

	return nullptr;
}

void AProjectileSplineActor::BeginPlay()
{
	Super::BeginPlay();
}
