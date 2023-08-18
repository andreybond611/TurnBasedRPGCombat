// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/Effects/OnFireEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/RPGCharacter.h"

UOnFireEffect::UOnFireEffect()
{
}

void UOnFireEffect::ApplyTo(AActor* InActor)
{
	Super::ApplyTo(InActor);

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(InActor))
	{
		GameplayTagHolder->AddTag(FGameplayTag::RequestGameplayTag("Effect.Debuff.OnFire"));
	}
}

void UOnFireEffect::Remove()
{
	Super::Remove();

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		GameplayTagHolder->RemoveTag(FGameplayTag::RequestGameplayTag("Effect.Debuff.OnFire"));
	}
}

void UOnFireEffect::ApplyVisuals()
{
	SpawnedVisual = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FireVisual, FVector::ZeroVector);

	TArray<UActorComponent*> FoundComponents = TargetActor->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), "CharacterMesh");
	if (!FoundComponents.IsEmpty())
	{
		const auto SkeletalMeshComponent = Cast<USkeletalMeshComponent>(FoundComponents[0]);
		if (ensureMsgf(SkeletalMeshComponent, TEXT("Component %s with \"CharacterMesh\" tag is not of type USkeletalMesh. Owner: %s"), //-V521
					   *FoundComponents[0]->GetName()),*TargetActor->GetName())
		{
			UNiagaraFunctionLibrary::OverrideSystemUserVariableSkeletalMeshComponent(SpawnedVisual, "SkeletalMesh", SkeletalMeshComponent);
		}
	}
}

void UOnFireEffect::RemoveVisuals()
{
	SpawnedVisual->DestroyInstance();
}

void UOnFireEffect::OnOwnerStartTurn()
{
	if (const auto Damageable = Cast<IDamageable>(TargetActor))
	{
		FDamage Damage;
		Damage.DamageNumber = DamagePerTurn;
		Damage.HitDirection = EHitDirection::Front;
		Damage.DamageType = EDamageType::Fire;

		Damageable->GetDamaged(Damage);
	}

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		const FGameplayTag FireSurfaceTag = FGameplayTag::RequestGameplayTag("Surface.Fire");
		if (GameplayTagHolder->HasMatchingGameplayTag(FireSurfaceTag))
		{
			TurnCount++;
		}
	}
}
