// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "RPGTeams.generated.h"

UENUM(BlueprintType)
enum class ERPGTeam : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Player UMETA(DisplayName = "Player"),
	Evil UMETA(DisplayName = "Evil"),
};

USTRUCT(BlueprintType, meta = (ScriptName = "Attitude"))
struct FTeamAttitude
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<ETeamAttitude::Type>> Attitude;

	FTeamAttitude() = default;

	FTeamAttitude(std::initializer_list<TEnumAsByte<ETeamAttitude::Type>> InAttitudes) :
	Attitude(MoveTemp(InAttitudes))
	{}
};



