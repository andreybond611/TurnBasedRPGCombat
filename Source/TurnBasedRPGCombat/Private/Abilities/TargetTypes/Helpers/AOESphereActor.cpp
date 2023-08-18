// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/TargetTypes/Helpers/AOESphereActor.h"

#include "Components/SphereComponent.h"

AAOESphereActor::AAOESphereActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");

	SetRootComponent(StaticMeshComponent);
	SphereCollision->SetupAttachment(StaticMeshComponent);

}

void AAOESphereActor::SetOverlapEnemy(const bool bOverlap)
{
	if (bOverlap)
	{
		SphereMaterialInstance->SetVectorParameterValue("Color", DetectedColor);
	}
	else
	{
		SphereMaterialInstance->SetVectorParameterValue("Color", DefaultColor);
	}
}

void AAOESphereActor::BeginPlay()
{
	Super::BeginPlay();

	SphereMaterialInstance = UMaterialInstanceDynamic::Create(SphereMaterial, this);

	SphereMaterialInstance->SetVectorParameterValue("Color", DefaultColor);
}
