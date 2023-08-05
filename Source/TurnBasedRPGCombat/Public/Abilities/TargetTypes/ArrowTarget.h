// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "ArrowTarget.generated.h"

/**
 *	Shows arrow on the ground
 */
UCLASS(BlueprintType)
class TURNBASEDRPGCOMBAT_API UArrowTarget : public UAbilityTargetState
{
	GENERATED_BODY()

	UArrowTarget();

public:
	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;
	virtual void StopTargeting() override;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* ArrowDecalMaterial;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* ArrowShaftDecalMaterial;
	UPROPERTY(EditAnywhere)
	FVector ArrowDecalSize = {32.f, 64.f, 64.f};
	UPROPERTY(EditAnywhere)
	FVector ArrowShaftDecalSize = {200.f, 10.f, 64.f};
	UPROPERTY(EditAnywhere)
	FLinearColor BlockedPathColor = FLinearColor::Red;
	UPROPERTY(EditAnywhere)
	FLinearColor HitEnemyColor = FLinearColor::Green;
	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor = FLinearColor::White;
	UPROPERTY(EditAnywhere)
	float MaxDistance = 1000.f;
	UPROPERTY(EditAnywhere)
	FText PathIsBlockedText = NSLOCTEXT("UI", "PathIsBlocked", "Path is Blocked");

private:
	UPROPERTY()
	UDecalComponent* ArrowDecal;
	UPROPERTY()
	UDecalComponent* ArrowShaftDecal;
	UPROPERTY()
	UMaterialInstanceDynamic* ArrowDecalInstance;
	UPROPERTY()
	UMaterialInstanceDynamic* ArrowShaftDecalInstance;

	float CharacterRadius;

	void DestroyDecals();
	void SpawnDecals(const FVector& Location);
	bool IsHitObstacle(const FVector& Location);
	void RemoveOutlineFromTargets();
	bool IsHitTarget(const FVector& Location);
	void SetDecalColor(FLinearColor Color);
	void CheckMaxDistance(const FHitResult& HitResult);
};
