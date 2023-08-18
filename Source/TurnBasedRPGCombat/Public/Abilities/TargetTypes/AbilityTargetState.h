// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilityTargetBase.h"
#include "Utility/TurnBasedTypes.h"
#include "AbilityTargetState.generated.h"

class IOutlinable;
class UAbility;
class ARPGCharacter;
class UNavigationSystemV1;
class ARPGPlayerController;

/**
 * Base class for targeting for a player
 */
UCLASS(Abstract, EditInlineNew, Blueprintable)
class TURNBASEDRPGCOMBAT_API UAbilityTargetState : public UAbilityTargetBase
{
	GENERATED_BODY()
public:
	virtual void StartTargeting(ARPGPlayerController* InPlayerController);
	virtual void TickTargetAbility(const FHitResult& CursorHitResult);
	virtual void StopTargeting();
	virtual void TargetNotHit();

	ECollisionChannel GetCollisionChannel() const { return CollisionChannel; }
	bool GetEnableRotating() const { return bEnableRotating; }

	bool IsAllowedExecute() const { return bAllowedExecute; }
	bool IsHitLocationWalkable(const FHitResult& HitResult);

	void EnableOutline(AActor* Actor, EOutlineColor Color);
	void EnableOutline(IOutlinable* Actor, EOutlineColor Color);
	void DisableOutline(AActor* Actor);
	void DisableOutline(IOutlinable* Actor);
	void DisableAllOutlines();

	FTarget GetTarget();

	void SetTargetActor(AActor* Actor) { Target.Actor = Actor; }
	void SetTargetLocation(FVector Location) { Target.Location = Location; }
	void SetMultipleTargetActors(TArray<AActor*> Actors) { Target.MultipleActors = Actors; }

protected:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECollisionChannel::ECC_GameTraceChannel1; /*MouseCursor*/

	UPROPERTY(EditAnywhere)
	bool bEnableRotating = true;

	bool bAllowedExecute = true;

	UPROPERTY()
	ARPGPlayerController* PlayerController;
	UPROPERTY()
	ARPGCharacter* ControlledCharacter;
	UPROPERTY()
	TArray<TScriptInterface<IOutlinable>> ActorsWithEnabledOutline;

	FTarget Target;
};
