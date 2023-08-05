// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "NavAreas/NavArea_Null.h"
#include "NavStaticMeshComponent.generated.h"

/**
 * Saves navigation area in a component instead of a static mesh. It helps when one static mesh asset is used by many
 * static mesh components and they need to have different NavAreas
 */
UCLASS(Blueprintable, ClassGroup = (Rendering, Common), hidecategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility),
	   editinlinenew, meta = (BlueprintSpawnableComponent))
class TURNBASEDRPGCOMBAT_API UNavStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	virtual void GetNavigationData(FNavigationRelevantData& Data) const override;

	void SetNavArea(TSubclassOf<class UNavArea> NavArea);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UNavArea> NavAreaClass = UNavArea_Null::StaticClass();
};
