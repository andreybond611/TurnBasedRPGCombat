// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDownTestActor.generated.h"

class ARPGCharacter;
class UInputAction;
UCLASS(BlueprintType, Blueprintable)
class ATopDownTestActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UInputAction* AbilityInput1;

	UPROPERTY(EditAnywhere)
	UInputAction* AbilityInput2;

	UPROPERTY(EditAnywhere)
	UInputAction* AbilityInput3;

	UPROPERTY(EditAnywhere)
	UInputAction* ExecuteAbilityInput;

	UPROPERTY(EditAnywhere)
	ARPGCharacter* Shinbi;

	UPROPERTY(EditAnywhere)
	ARPGCharacter* Yin;

	UPROPERTY(EditAnywhere)
	ARPGCharacter* Enemy;

	UPROPERTY(EditAnywhere)
	AActor* Pillar;

	UPROPERTY(EditAnywhere)
	FVector DashLocation;

	UPROPERTY(EditAnywhere)
	FVector WhirlwindLocation;
};
