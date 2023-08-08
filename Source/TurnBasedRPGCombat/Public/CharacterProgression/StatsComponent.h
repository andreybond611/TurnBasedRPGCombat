// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

class UGameStat;

/**
 * Changes and creates stats
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TURNBASEDRPGCOMBAT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	virtual void InitializeStats();

	void SetStatAsMaxStat(FName StatName, FName MaxStatName);

	UFUNCTION(BlueprintPure)
	UGameStat* FindStat(FName StatName);

	UFUNCTION(BlueprintPure)
	float Get(FName StatName);

	UFUNCTION(BlueprintCallable)
	void Set(FName StatName, float Value);

	UFUNCTION(BlueprintCallable)
	void Add(FName StatName, float Value);

	UFUNCTION(BlueprintCallable)
	void Remove(FName StatName, float Value);

	UFUNCTION(BlueprintCallable)
	UGameStat* CreateStat(FName StatName, FText TextName, float InitialValue, float MinValue = -9999.f , float MaxValue = 9999.f);

	UFUNCTION(BlueprintCallable)
	void AddMultiplier(FName StatName, float Multiplier);


	TMap<FName, UGameStat*> GetAllStats() const { return Stats; }
	void AddStatToCollection(FName StatName, UGameStat* Stat);
	void DeleteStatAt(FName StatName);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Instanced)
	TMap<FName, UGameStat*> Stats;
};
