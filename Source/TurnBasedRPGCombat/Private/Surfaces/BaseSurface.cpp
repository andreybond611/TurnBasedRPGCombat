// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Surfaces/BaseSurface.h"
#include "NavAreas/NavArea_Null.h"
#include "Surfaces/SurfaceActor.h"

UBaseSurface::UBaseSurface()
{
	NavAreaClass = UNavArea_Null::StaticClass();
}

void UBaseSurface::Init(ASurfaceActor* InSurfaceActor)
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

void UBaseSurface::OnSurfaceIntersect(ASurfaceActor* OtherSurface)
{

}

void UBaseSurface::ChangeSurfaceType(const TSubclassOf<UBaseSurface> InSurfaceType)
{
	SurfaceActor->SetSurfaceType(InSurfaceType);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]
	{
		for (ASurfaceActor* OverlappingSurface : SurfaceActor->GetOverlappingSurfaces())
		{
			const ASurfaceActor* OverlappingSurfaceActor = CastChecked<ASurfaceActor>(OverlappingSurface);
			OverlappingSurfaceActor->GetSurface()->OnSurfaceIntersect(SurfaceActor);
		}
	});

	
}
