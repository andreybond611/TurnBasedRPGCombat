// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPGMovementComponent.generated.h"

class UGameStat;

/**
 * Adds walk speed stat
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API URPGMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	URPGMovementComponent();
	UGameStat* GetWalkSpeedStat() const { return WalkSpeedStat; }

protected:
	virtual void PostLoad() override;

private:
	UPROPERTY(EditAnywhere, Category = "Walking", Instanced)
	UGameStat* WalkSpeedStat;

	void OnMoveSpeedStatChanged(float MoveSpeed);
};
