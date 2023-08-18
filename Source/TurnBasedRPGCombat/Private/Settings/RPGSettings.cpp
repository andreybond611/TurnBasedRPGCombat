// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "Settings/RPGSettings.h"

URPGSettings::URPGSettings()
{
	using TA = ETeamAttitude::Type;
	TeamAttitudes = {
		{TA::Friendly, TA::Neutral, TA::Neutral}, // Neutral
		{TA::Neutral, TA::Friendly, TA::Hostile}, // Player
		{TA::Neutral, TA::Hostile, TA::Friendly}  // Evil
	};
}

const URPGSettings* URPGSettings::Get()
{
	return GetDefault<URPGSettings>();
}

ETeamAttitude::Type URPGSettings::GetAttitude(const FGenericTeamId Of, const FGenericTeamId Towards)
{
	const TArray<FTeamAttitude>& TeamAttitudes = Get()->TeamAttitudes;
	const bool bOfValid = TeamAttitudes.IsValidIndex(Of.GetId());
	const bool bTowardsValid = TeamAttitudes.IsValidIndex(Towards.GetId());

	if (bOfValid && bTowardsValid)
	{
		const TArray<TEnumAsByte<ETeamAttitude::Type>>& Attitudes = TeamAttitudes[Of.GetId()].Attitude;
		if (Attitudes.IsValidIndex(Towards.GetId()))
		{
			return Attitudes[Towards.GetId()];
		}
	}
	return ETeamAttitude::Neutral;
}
