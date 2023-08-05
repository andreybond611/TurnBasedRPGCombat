// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "AI/TurnBasedAIController.h"

#include "ActorComponents/TurnBasedComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/RPGCharacter.h"
#include "TurnBased/TurnBasedBattle.h"


void ATurnBasedAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (Behavior)
	{
		RunBehaviorTree(Behavior);
	}

	auto* ControlledCharacter = Cast<ARPGCharacter>(InPawn);
	if (ensure(ControlledCharacter))
	{
		ControlledCharacter->GetTurnBasedComponent()->OnEnterBattle.AddDynamic(this, &ATurnBasedAIController::EnterBattle);
		ControlledCharacter->GetTurnBasedComponent()->OnExitBattle.AddDynamic(this, &ATurnBasedAIController::ExitBattle);

		ControlledCharacter->GetTurnBasedComponent()->OnTurnBegin.AddDynamic(this, &ATurnBasedAIController::StartTurn);
		ControlledCharacter->GetTurnBasedComponent()->OnTurnEnd.AddDynamic(this, &ATurnBasedAIController::EndTurn);
	}
}

void ATurnBasedAIController::StartTurn()
{
	GetBlackboardComponent()->SetValueAsBool("CurrentTurn", true);
}

void ATurnBasedAIController::EndTurn()
{
	GetBlackboardComponent()->SetValueAsBool("CurrentTurn", false);
}

void ATurnBasedAIController::SetPlayerControlled(bool bPlayerControlled)
{
	GetBlackboardComponent()->SetValueAsBool("PlayerControlled", bPlayerControlled);
}

void ATurnBasedAIController::EnterBattle()
{
	auto TurnBasedComponent = GetPawn<ARPGCharacter>()->GetTurnBasedComponent();
	ATurnBasedBattle* CurrentBattle = TurnBasedComponent->GetCurrentBattle();

	GetBlackboardComponent()->SetValueAsObject("CurrentBattle", CurrentBattle);
}

void ATurnBasedAIController::ExitBattle()
{
	GetBlackboardComponent()->SetValueAsObject("CurrentBattle", nullptr);
}
