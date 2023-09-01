// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Surfaces/FireSurface.h"

#include "Abilities/Effects/Effect.h"
#include "Abilities/Effects/EffectComponent.h"
#include "Interaction/Damageable.h"
#include "Utility/TurnBasedTypes.h"

void UFireSurface::OnActorEntered(AActor* Actor)
{
	DistancePassedOnSurface.Add(Actor, 0.f);

	if (const auto Damageable = Cast<IDamageable>(Actor))
	{
		FDamage Damage;
		Damage.DamageNumber = DamageOnMove;
		Damage.HitDirection = EHitDirection::Front;
		Damage.DamageType = EDamageType::Fire;

		Damageable->GetDamaged(Damage);
	}

	if (const auto EffectComponent = Actor->FindComponentByClass<UEffectComponent>())
	{
		UEffect* Effect = NewObject<UEffect>(Actor, *SurfaceEffectClass);
		EffectComponent->AddEffect(Effect);
	}

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(Actor))
	{
		if (!GameplayTagHolder->HasMatchingGameplayTag(SurfaceTag))
		{
			GameplayTagHolder->AddTag(SurfaceTag);
		}
	}
}

void UFireSurface::SurfaceTick(float DeltaTime)
{
	TArray<AActor*> Actors;

	DistancePassedOnSurface.GenerateKeyArray(Actors);

	for (AActor* Actor : Actors)
	{
		const auto Character = Cast<ARPGCharacter>(Actor);
		if (Character && Character->IsMoving())
		{
			const float PassedDistance = Character->GetPassedDistanceSinceLastFrame();
			float NewDistance = DistancePassedOnSurface.FindChecked(Actor) + PassedDistance;

			if (NewDistance > DistanceThreshold)
			{
				Character->GetDamaged({DamageOnMove, EHitDirection::None});
				UEffect* Effect = NewObject<UEffect>(Actor, *SurfaceEffectClass);
				Character->ApplyEffect(Effect);
				NewDistance -= DistanceThreshold;
			}

			DistancePassedOnSurface.Add(Actor, NewDistance);
		}
	}
}

void UFireSurface::OnActorLeft(AActor* Actor)
{
	DistancePassedOnSurface.Remove(Actor);

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(Actor))
	{
		if (GameplayTagHolder->HasMatchingGameplayTag(SurfaceTag))
		{
			GameplayTagHolder->RemoveTag(SurfaceTag);
		}
	}
}

void UFireSurface::OnSurfaceIntersect(ASurfaceActor* OtherSurface)
{
}
