// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Surfaces/OilSurface.h"
#include "Abilities/Effects/Effect.h"
#include "Abilities/Effects/EffectComponent.h"
#include "Surfaces/FireSurface.h"
#include "Surfaces/SurfaceActor.h"

void UOilSurface::OnActorEntered(AActor* Actor)
{
	if (const auto EffectComponent = Actor->FindComponentByClass<UEffectComponent>())
	{
		const auto Effect = NewObject<UEffect>(this, *SurfaceEffectClass);
		EffectComponent->AddEffect(Effect);
	}
}

void UOilSurface::OnActorLeft(AActor* Actor)
{
	
}

void UOilSurface::OnSurfaceIntersect(ASurfaceActor* OtherSurface)
{
	if (OtherSurface->GetSurface()->IsA(UFireSurface::StaticClass()))
	{
		const auto SurfaceType = OtherSurface->GetSurfaceType();
		ChangeSurfaceType(SurfaceType);
	}
}
