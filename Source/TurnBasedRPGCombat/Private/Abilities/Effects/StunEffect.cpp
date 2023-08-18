// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/Effects/StunEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/RPGCharacter.h"

UStunEffect::UStunEffect()
{
}

void UStunEffect::ApplyTo(AActor* InActor)
{
	Super::ApplyTo(InActor);

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(InActor))
	{
		GameplayTagHolder->AddTag(FGameplayTag::RequestGameplayTag("Effect.Debuff.Stun"));
	}
}

bool UStunEffect::CanApplyTo(AActor* InActor)
{
	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(InActor))
	{
		return !GameplayTagHolder->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Effect.Immunity.Stun"));
	}
	return true;
}

void UStunEffect::Remove()
{
	Super::Remove();

	if (const auto GameplayTagHolder = Cast<IGameplayTagHolder>(TargetActor))
	{
		GameplayTagHolder->RemoveTag(FGameplayTag::RequestGameplayTag("Effect.Debuff.Stun"));
	}
}

void UStunEffect::ApplyVisuals()
{
	TArray<UActorComponent*> FoundComponents = TargetActor->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), "CharacterMesh");
	if (!FoundComponents.IsEmpty())
	{
		const auto SkeletalMeshComponent = Cast<USkeletalMeshComponent>(FoundComponents[0]);
		if (ensureMsgf(SkeletalMeshComponent, TEXT("Component %s with \"CharacterMesh\" tag is not of type USkeletalMesh. Owner: %s"), //-V521
			*FoundComponents[0]->GetName()), *TargetActor->GetName())
		{
			SpawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(StunVisual, SkeletalMeshComponent, "AboveHead", FVector(), FRotator(90.f, 0.f, 0.f),
																		 EAttachLocation::SnapToTarget, false);
		}
	}
}

void UStunEffect::RemoveVisuals()
{
	SpawnedSystem->DestroyInstance();
}
