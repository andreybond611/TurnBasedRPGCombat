// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Interaction/RPGTeams.h"
#include "RPGSettings.generated.h"

/**
 *
 */
UCLASS(Config = Game, DefaultConfig)
class TURNBASEDRPGCOMBAT_API URPGSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	URPGSettings();

	UPROPERTY(Category = "Artificial Intelligence", EditAnywhere, BlueprintReadOnly, Config)
	TArray<FTeamAttitude> TeamAttitudes;

	static const URPGSettings* Get();

	UFUNCTION(Category = "Artificial Intelligence", BlueprintPure)
	static ETeamAttitude::Type GetAttitude(FGenericTeamId Of, FGenericTeamId Towards);
};
