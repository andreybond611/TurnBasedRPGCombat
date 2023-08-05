// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Surfaces/BaseSurface.h"
#include "NavAreas/NavArea_Null.h"
#include "Surfaces/RPGSurface.h"

UBaseSurface::UBaseSurface()
{
	NavAreaClass = UNavArea_Null::StaticClass();
}

void UBaseSurface::Init(ARPGSurface* InSurfaceActor)
{
	SurfaceActor = InSurfaceActor;
}

void UBaseSurface::OnActorEntered(AActor* Actor)
{

}

void UBaseSurface::SurfaceTick(float DeltaTime)
{

}

void UBaseSurface::OnActorLeft(AActor* Actor)
{

}

void UBaseSurface::OnSurfaceIntersect(ARPGSurface* OtherSurface)
{

}

void UBaseSurface::ChangeSurfaceType(TSubclassOf<UBaseSurface> InSurfaceType)
{
	SurfaceActor->SetSurfaceType(InSurfaceType);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]
	{
		for (ARPGSurface* OverlappingSurface : SurfaceActor->GetOverlappingSurfaces())
		{
			ARPGSurface* OverlappingSurfaceActor = CastChecked<ARPGSurface>(OverlappingSurface);
			OverlappingSurfaceActor->GetSurface()->OnSurfaceIntersect(SurfaceActor);
		}
	});

	
}
