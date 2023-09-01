// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/Effects/PoisonedEffect.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interaction/Damageable.h"
#include "Interaction/GameplayTagHolder.h"
#include "Utility/TurnBasedTypes.h"

UPoisonedEffect::UPoisonedEffect()
{
}

void UPoisonedEffect::ApplyTo(AActor* InActor)
{
	Super::ApplyTo(InActor);

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(InActor))
	{
		GameplayTagHolder->AddTag(FGameplayTag::RequestGameplayTag("Effect.Debuff.Poisoned"));
	}
}

void UPoisonedEffect::Remove()
{
	Super::Remove();

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		GameplayTagHolder->RemoveTag(FGameplayTag::RequestGameplayTag("Effect.Debuff.Poisoned"));
	}
}

void UPoisonedEffect::ApplyVisuals()
{
	SpawnedVisual = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PoisonCharacterEffect, FVector::ZeroVector);

	TArray<UActorComponent*> FoundComponents = TargetActor->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), "CharacterMesh");
	if (!FoundComponents.IsEmpty())
	{
		const auto SkeletalMeshComponent = Cast<USkeletalMeshComponent>(FoundComponents[0]);
		if (ensureMsgf(SkeletalMeshComponent, TEXT("Component %s with \"CharacterMesh\" tag is not of type USkeletalMesh. Owner: %s"), //-V521
			*FoundComponents[0]->GetName()), *TargetActor->GetName())
		{
			UNiagaraFunctionLibrary::OverrideSystemUserVariableSkeletalMeshComponent(SpawnedVisual, "SkeletalMesh", SkeletalMeshComponent);
		}
	}
}

void UPoisonedEffect::RemoveVisuals()
{
	SpawnedVisual->DestroyInstance();
}

void UPoisonedEffect::OnOwnerStartTurn()
{
	if (const auto Damageable = Cast<IDamageable>(TargetActor))
	{
		FDamage Damage;
		Damage.DamageNumber = DamagePerTurn;
		Damage.HitDirection = EHitDirection::None;
		Damage.DamageType = EDamageType::Poison;

		Damageable->GetDamaged(Damage);
	}

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		const FGameplayTag PoisonSurfaceTag = FGameplayTag::RequestGameplayTag("Surface.Poison");
		if (GameplayTagHolder->HasMatchingGameplayTag(PoisonSurfaceTag))
		{
			TurnCount++;
		}
	}
}
