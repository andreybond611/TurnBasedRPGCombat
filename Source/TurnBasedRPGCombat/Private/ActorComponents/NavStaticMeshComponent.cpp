// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "ActorComponents/NavStaticMeshComponent.h"
#include "NavCollision.h"
#include "AI/NavigationSystemBase.h"
#include "AI/Navigation/NavCollisionBase.h"

void UNavStaticMeshComponent::GetNavigationData(FNavigationRelevantData& Data) const
{
	UMeshComponent::GetNavigationData(Data);

	const FVector Scale3D = GetComponentToWorld().GetScale3D();

	// Navigation data will get refreshed once async static mesh compilation finishes
	if (!Scale3D.IsZero() && GetStaticMesh() && !GetStaticMesh()->IsCompiling() && GetStaticMesh()->GetNavCollision())
	{
		UNavCollisionBase* NavCollisionBase = GetStaticMesh()->GetNavCollision();
		UNavCollision* NavCollision = Cast<UNavCollision>(NavCollisionBase);
		if (!NavCollision)
		{
			return;
		}

		NavCollision->AreaClass = NavAreaClass;
		const bool bExportAsObstacle = bOverrideNavigationExport ? bForceNavigationObstacle : NavCollision->IsDynamicObstacle();

		if (bExportAsObstacle)
		{
			NavCollision->GetNavigationModifier(Data.Modifiers, GetComponentTransform());
		}
	}
}

void UNavStaticMeshComponent::SetNavArea(TSubclassOf<UNavArea> NavArea)
{
	NavAreaClass = NavArea;
	FNavigationSystem::UpdateComponentData(*this);
}
