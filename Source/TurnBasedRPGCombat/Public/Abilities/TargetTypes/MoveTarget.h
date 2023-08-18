// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "NavigationData.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "MoveTarget.generated.h"

class IDynamicObstacle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveToActorSuccess, AActor*, TargetActor);

/**
 * Displays a potential path on a ground
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UMoveTarget : public UAbilityTargetState
{
	GENERATED_BODY()
public:
	UMoveTarget();

	virtual void StartTargeting(ARPGPlayerController* InPlayerController) override;
	void AddTargetInfoActor(UNavigationPath* NavigationPath, AActor* Actor);
	void ShowDestination(const FHitResult& HitResult);
	virtual void TickTargetAbility(const FHitResult& CursorHitResult) override;
	virtual void StopTargeting() override;

	void SetShowPath(const bool bInShowPath) { bShowPath = bInShowPath; }
	bool IsMoveAllowed() const { return bAllowMove; }

	UPROPERTY(BlueprintAssignable)
	FOnMoveToActorSuccess OnMoveToActorSuccess;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bShowPath = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bAllowMove = true;

	UPROPERTY(EditAnywhere)
	float AcceptanceDistance = 120.f;

	UPROPERTY(EditAnywhere)
	float DistanceBetweenPathDecals = 50.f;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* PointDecalMaterial;
	UPROPERTY(EditAnywhere)
	FVector PointDecalSize = FVector(100.f, 7.f, 7.f);
	UPROPERTY(EditAnywhere)
	UMaterialInterface* LastPointDecalMaterial;

	UPROPERTY()
	UNavigationSystemV1* NavSystem;
	UPROPERTY()
	TArray<UDecalComponent*> PathDecals;
	UPROPERTY()
	UDecalComponent* DestinationDecalComponent;
	UPROPERTY()
	TScriptInterface<IDynamicObstacle> DynamicObstacle;

	UFUNCTION()
	void NavigationGenerationFinished(ANavigationData* NavData);

	void SpawnDecalsOnPath(UNavigationPath* NavigationPath);
	UNavigationPath* FindPathToActor(AActor* Actor);
	UNavigationPath* FindPathToLocation(const FVector& Location);
	void AddTargetInfoLocation(UNavigationPath* DestinationPath);
	void RemoveDecals();
	void RemoveDestination();
	void DisableDynamicObstacle(AActor* Actor);
	void EnableDynamicObstacle();
};
