// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "SphereAroundTarget.generated.h"

/**
 * Shows circle on the ground
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API USphereAroundTarget : public UAbilityTargetState
{
	GENERATED_BODY()
public:
	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;
	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StopTargeting() override;

private:
	UPROPERTY(EditAnywhere)
	UMaterialInterface* CircleMaterial;
	UPROPERTY(EditAnywhere)
	FVector DecalSize{128.f, 256.f, 256.f};
	UPROPERTY(EditAnywhere)
	float AbilityRadius = 250.f;

	UPROPERTY()
	UMaterialInstanceDynamic* CircleMaterialInstance;
	UPROPERTY()
	UDecalComponent* CircleDecal;

	UPROPERTY(EditAnywhere)
	FLinearColor OuterCircleEnemyColor;
	UPROPERTY(EditAnywhere)
	FLinearColor InnerCircleEnemyColor;
	UPROPERTY(EditAnywhere)
	FLinearColor OuterCircleDefaultColor;
	UPROPERTY(EditAnywhere)
	FLinearColor InnerCircleDefaultColor;

	bool SphereTraceForPawnsAround(FVector OriginLocation);
	void SetCircleColor(FLinearColor OuterColor, FLinearColor InnerColor);
	void HighlightActor(AActor* Actor);
};
