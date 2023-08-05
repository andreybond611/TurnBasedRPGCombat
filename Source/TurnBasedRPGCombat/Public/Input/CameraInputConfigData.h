// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CameraInputConfigData.generated.h"

class UInputAction;
/**
 * Stores input actions for player's camera for native binding
 */
UCLASS(Blueprintable)
class TURNBASEDRPGCOMBAT_API UCameraInputConfigData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* CameraRotateYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AllowCameraRotate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* CameraMoveVertical;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* CameraMoveOrthogonal;
};
