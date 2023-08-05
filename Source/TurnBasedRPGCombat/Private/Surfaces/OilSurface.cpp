// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Surfaces/OilSurface.h"
#include "Abilities/Effects/Effect.h"
#include "Abilities/Effects/EffectComponent.h"
#include "Surfaces/FireSurface.h"
#include "Surfaces/RPGSurface.h"

void UOilSurface::OnActorEntered(AActor* Actor)
{
	if (auto EffectComponent = Actor->FindComponentByClass<UEffectComponent>())
	{
		auto Effect = NewObject<UEffect>(this, *SurfaceEffectClass);
		EffectComponent->AddEffect(Effect);
	}
}

void UOilSurface::OnActorLeft(AActor* Actor)
{
	
}

void UOilSurface::OnSurfaceIntersect(ARPGSurface* OtherSurface)
{
	if (OtherSurface->GetSurface()->IsA(UFireSurface::StaticClass()))
	{
		auto SurfaceType = OtherSurface->GetSurfaceType();
		ChangeSurfaceType(SurfaceType);
	}
}
