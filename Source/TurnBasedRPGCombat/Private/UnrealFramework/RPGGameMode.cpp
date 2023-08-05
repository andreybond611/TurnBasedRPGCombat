// Copyright 2022 Andrei Bondarenko. All rights reserved


#include "UnrealFramework/RPGGameMode.h"

#include "GenericTeamAgentInterface.h"
#include "Settings/RPGSettings.h"

void ARPGGameMode::StartPlay()
{
	Super::StartPlay();

	FGenericTeamId::SetAttitudeSolver(&URPGSettings::GetAttitude);
}
