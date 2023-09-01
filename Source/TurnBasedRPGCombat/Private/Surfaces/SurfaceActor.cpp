// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Surfaces/SurfaceActor.h"
#include "ActorComponents/NavStaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Surfaces/BaseSurface.h"

ASurfaceActor::ASurfaceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	SetRootComponent(Origin);

	CollisionShape = CreateDefaultSubobject<UNavStaticMeshComponent>("CollisionShape");
	Decal = CreateDefaultSubobject<UDecalComponent>("Decal");

	CollisionShape->SetupAttachment(Origin);
	Decal->SetupAttachment(Origin);

	CollisionShape->SetCollisionObjectType(ECC_GameTraceChannel4 /*Surface*/);
	CollisionShape->SetCollisionResponseToAllChannels(ECR_Ignore);

	// for navmesh to see this we need to set response to either pawn or vehicle to block
	CollisionShape->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);

	CollisionShape->SetCollisionResponseToChannel(ECC_GameTraceChannel4 /*Surface*/, ECR_Overlap);
	CollisionShape->SetCollisionResponseToChannel(ECC_GameTraceChannel5 /*CharacterFeet*/, ECR_Overlap);

	CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &ASurfaceActor::OnOverlapBegin);
	CollisionShape->OnComponentEndOverlap.AddDynamic(this, &ASurfaceActor::OnOverlapEnd);
}

void ASurfaceActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								 bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ASurfaceActor>())
	{
		const auto OverlappingSurface = CastChecked<ASurfaceActor>(OtherActor);
		OverlappingSurfaces.Add(OverlappingSurface);
		Surface->OnSurfaceIntersect(OverlappingSurface);
		return;
	}

	Surface->OnActorEntered(OtherActor);
}

void ASurfaceActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Surface->OnActorLeft(OtherActor);
}

void ASurfaceActor::SetSurfaceSize(const float InSize)
{
	SurfaceSize = InSize;
	CalculateSize();
}

void ASurfaceActor::SetSurfaceType(const TSubclassOf<UBaseSurface> InSurfaceType)
{
	static const FName DecalShapeParameterName = "DecalShape";

	if (InSurfaceType)
	{
		Surface = NewObject<UBaseSurface>(this, *InSurfaceType);
		Surface->Init(this);
		NavAreaClass = Surface->GetNavArea();

		CollisionShape->SetNavArea(NavAreaClass);

		DecalMaterialInstance = UMaterialInstanceDynamic::Create(Surface->GetMaterial(), this);
		DecalMaterialInstance->SetTextureParameterValue(DecalShapeParameterName, TextureShape);
		Decal->SetDecalMaterial(DecalMaterialInstance);

		SurfaceMaterial = Surface->GetMaterial();
		SurfaceType = InSurfaceType;
	}
}

void ASurfaceActor::CalculateSize()
{
	if (Decal && CollisionShape)
	{
		Decal->DecalSize = FVector{ 200.f, SurfaceSize, SurfaceSize };
		const float MeshScale = SurfaceSize / ShapeSize;
		CollisionShape->SetRelativeScale3D(FVector{ MeshScale, MeshScale, 1.f });
	}
}

void ASurfaceActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	static const FName DecalShapeParameterName = "DecalShape";

	DecalMaterialInstance = UMaterialInstanceDynamic::Create(SurfaceMaterial, this);
	DecalMaterialInstance->SetTextureParameterValue(DecalShapeParameterName, TextureShape);
	Decal->SetDecalMaterial(DecalMaterialInstance);
	CollisionShape->SetMaterial(0, CollisionMeshMaterial);

	SetSurfaceType(SurfaceType);

	CalculateSize();
}

void ASurfaceActor::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Surface)
	{
		Surface->SurfaceTick(DeltaTime);
	}
}
