// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AIAbilityTarget.h"
#include "EQSAIAbilityTarget.generated.h"

class UEnvQuery;
/**
 * Sets EnvQuery
 */
UCLASS()
class TURNBASEDRPGCOMBAT_API UEQSAIAbilityTarget : public UAIAbilityTarget
{
	GENERATED_BODY()
public:
	virtual void Init(AActor* InAbilityOwner, ATurnBasedBattle* InContextBattle, UBlackboardComponent* InBlackboard) override;
	void SetEnvQuery(UEnvQuery* Query);
	
private:
	UPROPERTY(EditAnywhere)
	UEnvQuery* EnvQuery;

	inline static const FName EnvQueryName = "EnvQuery";
};
