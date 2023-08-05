// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#if WITH_AUTOMATION_TESTS

#include "ShinbiAbilities.spec.h"
#include "Abilities/Effects/EffectComponent.h"
#include "Abilities/Effects/StunEffect.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Tests/AutomationCommon.h"
#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "TestActors/TopDownTestActor.h"
#include "EnhancedPlayerInput.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"

// clang-format off

BEGIN_DEFINE_SPEC(FShinbiAbilitiesSpec, "TurnBasedRPGCombat.ShinbiAbilities",
				  EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

UWorld* World = nullptr;
const char* MapName = "/Game/TurnBasedRPGCombat/Maps/Map_Test";
APlayerController* PlayerController;
APawn* PlayerPawn;
ATopDownTestActor* TestActor;
UEnhancedPlayerInput* PlayerInput;

END_DEFINE_SPEC(FShinbiAbilitiesSpec)

void FShinbiAbilitiesSpec::Define()
{
	LatentBeforeEach([this](const FDoneDelegate& Done)
	{
		AutomationOpenMap(MapName);
		World = Test::GetGameWorld();
		//check(World)

		PlayerController = World->GetFirstPlayerController();
		PlayerPawn = PlayerController->GetPawn();

		auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		PlayerInput = InputSubsystem->GetPlayerInput();

		FTimerHandle MapInitializationTimerHandle;
		World->GetTimerManager().SetTimer(MapInitializationTimerHandle, [this, Done]()
		{
			for (TActorIterator<ATopDownTestActor> It(World); It; ++It)
			{
				TestActor = *It;
				break;
			}
			Done.Execute();
		}, 0.1f, false);
	});

	Describe("Shinbi Dash", [this]()
	{
		LatentIt("Dash should execute", [this](const FDoneDelegate& Done)
		{
			UInputAction* InputAction = TestActor->AbilityInput1;
			FInputActionValue ActionValue(true);
			PlayerInput->InjectInputForAction(InputAction, ActionValue);

			FVector2D ScreenLocation;
			TestTrueExpr(PlayerController->ProjectWorldLocationToScreen(TestActor->DashLocation, ScreenLocation));

			FTimerHandle ReadyAnimationEndTimerHandle;
			World->GetTimerManager().SetTimer(ReadyAnimationEndTimerHandle, [this, Done, ScreenLocation]()
			{
				FVector ShinbiInitialLocation = TestActor->Shinbi->GetActorLocation();
				float EnemyInitialHP = TestActor->Enemy->Stats()->Get(SN_Health);

				PlayerController->SetMouseLocation(ScreenLocation.X, ScreenLocation.Y);

				FTimerHandle MouseLocationTimerHandle;
				World->GetTimerManager().SetTimer(MouseLocationTimerHandle, [this, Done]()
				{
					UInputAction* InputAction = TestActor->ExecuteAbilityInput;
					FInputActionValue ActionValue(true);
					PlayerInput->InjectInputForAction(InputAction, ActionValue);
				}, 0.1f, false);

				FTimerHandle ExecuteAbilityTimerHandle;
				World->GetTimerManager().SetTimer(ExecuteAbilityTimerHandle, [this, Done, ShinbiInitialLocation, EnemyInitialHP]()
				{
					TestNotEqual("Shinbi location should change", ShinbiInitialLocation, TestActor->Shinbi->GetActorLocation());
					TestTrue("Enemy should take damage", TestActor->Enemy->Stats()->Get(SN_Health) < EnemyInitialHP);
					TestTrue("Enemy should be stunned", TestActor->Enemy->GetEffectComponent()->HasEffect<UStunEffect>());
					Done.Execute();
				}, 1.f, false);
			}, 1.7f, false);
			
		});
	});

	Describe("Whirlwind ability", [this]()
	{
		LatentIt("Whirlwind should execute", [this](const FDoneDelegate& Done)
		{
			TestActor->Shinbi->MoveToLocation(TestActor->WhirlwindLocation);

			FTimerHandle MoveTimerHandle;
			World->GetTimerManager().SetTimer(MoveTimerHandle, [this, Done]()
			{
				float EnemyInitialHP = TestActor->Enemy->Stats()->Get(SN_Health);

				UInputAction* AbilityInput = TestActor->AbilityInput2;
				FInputActionValue ActionValue(true);
				PlayerInput->InjectInputForAction(AbilityInput, ActionValue);

				FTimerHandle AnimationTimerHandle;
				World->GetTimerManager().SetTimer(AnimationTimerHandle, [this, Done]()
				{
					UInputAction* InputAction = TestActor->ExecuteAbilityInput;
					FInputActionValue ActionValue(true);
					PlayerInput->InjectInputForAction(InputAction, ActionValue);
				}, 1.f, false);
				

				FTimerHandle AbilityTimerHandle;
				World->GetTimerManager().SetTimer(AbilityTimerHandle, [this, Done, EnemyInitialHP]()
				{
					TestTrue("Enemy should take damage", TestActor->Enemy->Stats()->Get(SN_Health) < EnemyInitialHP);
					Done.Execute();
				}, 2.f, false);
			}, 1.f, false);
		});
	});


}

// clang-format on

#endif