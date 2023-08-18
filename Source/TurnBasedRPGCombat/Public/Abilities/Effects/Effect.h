// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Effect.generated.h"

class ARPGCharacter;
/**
 * Base effect class. Effects are applied to actors through UEffectComponent->AddEffect()
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class TURNBASEDRPGCOMBAT_API UEffect : public UObject
{
	GENERATED_BODY()
public:
	void ReduceTurn();
	int32 GetTurnCount() const { return TurnCount; }
	void SetTurnCount(const int32 InTurnCount) { TurnCount = InTurnCount; }

protected:
	friend class UEffectComponent;

	virtual void ApplyTo(AActor* InActor);
	virtual bool CanApplyTo(AActor* InActor);
	virtual void Remove();
	virtual bool CanRemove();

	/** When the same effect already applied they stack */
	virtual void Stack(UEffect* Effect);

	virtual void ApplyVisuals();
	virtual void RemoveVisuals();

	virtual void OnOwnerStartTurn();

	TArray<FGameplayTag> GameplayTags;
	FGameplayTag GameplayTag;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY(EditAnywhere)
	bool bInfinite = false;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", EditCondition = "!bInfinite"))
	int32 TurnCount = 1;
};
