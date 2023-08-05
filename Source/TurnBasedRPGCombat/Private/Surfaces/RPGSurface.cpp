// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Surfaces/RPGSurface.h"
#include "ActorComponents/NavStaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Surfaces/BaseSurface.h"

ARPGSurface::ARPGSurface()
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

	CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &ARPGSurface::OnOverlapBegin);
	CollisionShape->OnComponentEndOverlap.AddDynamic(this, &ARPGSurface::OnOverlapEnd);
}

void ARPGSurface::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								 bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ARPGSurface>())
	{
		auto OverlappingSurface = CastChecked<ARPGSurface>(OtherActor);
		OverlappingSurfaces.Add(OverlappingSurface);
		Surface->OnSurfaceIntersect(OverlappingSurface);
		return;
	}

	Surface->OnActorEntered(OtherActor);
}

void ARPGSurface::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Surface->OnActorLeft(OtherActor);
}

void ARPGSurface::SetSurfaceSize(float InSize)
{
	SurfaceSize = InSize;
	CalculateSize();
}

void ARPGSurface::SetSurfaceType(TSubclassOf<UBaseSurface> InSurfaceType)
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

void ARPGSurface::CalculateSize()
{
	Decal->DecalSize = FVector{200.f, SurfaceSize, SurfaceSize};
	float MeshScale = SurfaceSize / ShapeSize;
	CollisionShape->SetRelativeScale3D(FVector{MeshScale, MeshScale, 1.f});
}

void ARPGSurface::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	static const FName DecalShapeParameterName = "DecalShape";

	DecalMaterialInstance = UMaterialInstanceDynamic::Create(SurfaceMaterial, this);
	DecalMaterialInstance->SetTextureParameterValue(DecalShapeParameterName, TextureShape);
	Decal->SetDecalMaterial(DecalMaterialInstance);

	SetSurfaceType(SurfaceType);

	CalculateSize();
}

void ARPGSurface::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Surface)
	{
		Surface->SurfaceTick(DeltaTime);
	}
}
