// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "Characters/PlayerCamera.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/CameraInputConfigData.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

APlayerCamera::APlayerCamera()
{
	Camera = CreateDefaultSubobject<UCameraComponent>("TopDownCamera");
	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovementComponent");
	CameraRootComponent = CreateDefaultSubobject<UArrowComponent>("CameraRoot");

	SetRootComponent(CameraRootComponent);

	Camera->SetupAttachment(CameraRootComponent);
	Camera->FieldOfView = 90.f;
	Camera->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	Camera->SetRelativeLocation({-800.f, 0.f, 0.f});

	TargetCameraPitchRotation = CalculateAlphaFromCameraGroundDistance(MinCameraPitchRotation, MaxCameraPitchRotation);
	TargetCameraDistanceFromFocusPoint = CalculateAlphaFromCameraGroundDistance(MinCameraDistanceFromFocusPoint, MaxCameraDistanceFromFocusPoint);
}

float APlayerCamera::CalculateAlphaFromCameraGroundDistance(float MinValue, float MaxValue)
{
	float MaxDistance = MaxCameraHeight - MinCameraHeight;
	float CurrentDistance = CameraHeight - MinCameraHeight;
	float Alpha = CurrentDistance / MaxDistance;

	return FMath::Lerp(MinValue, MaxValue, Alpha);
}

void APlayerCamera::StartFollowing(AActor* ActorToFollow, bool bInDisableFollowWhenOnTarget)
{
	FollowTarget = ActorToFollow;
	bDisableFollowWhenOnTarget = bInDisableFollowWhenOnTarget;

	// see Follow()
}

void APlayerCamera::StopFollowing()
{
	FollowTarget = nullptr;
}

void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	GetController()->SetControlRotation(FRotator(-50.f, 0.f, 0.f));
}

void APlayerCamera::InterpCameraVerticalMove(float DeltaSeconds, float TargetGroundDistance)
{
	float InterpResult = FMath::FInterpTo(GetActorLocation().Z, TargetGroundDistance, DeltaSeconds, VerticalMovementSpeed);
	FVector CameraLocationToSet = GetActorLocation();
	CameraLocationToSet.Z = InterpResult;
	SetActorLocation(CameraLocationToSet);
}

void APlayerCamera::TraceGround(float DeltaSeconds)
{
	FVector TargetLocation = GetActorLocation() + FVector::DownVector * MaxCameraHeightTraceDistance;

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2 /*GroundOnly*/);
	TArray<AActor*> ActorsToIgnore;

	if (UKismetSystemLibrary::LineTraceSingle(this, GetActorLocation(), TargetLocation, TraceType, false, ActorsToIgnore, EDrawDebugTrace::None,
											  HitResult, true))
	{
		float TargetFloorDistance = HitResult.Location.Z + CameraHeight;
		InterpCameraVerticalMove(DeltaSeconds, TargetFloorDistance);
	}
}

void APlayerCamera::Rotate(float DeltaSeconds)
{
	float NewPitchRotation = FMath::FInterpTo(GetControlRotation().Pitch, TargetCameraPitchRotation, DeltaSeconds, PitchRotationSpeed);

	float NewYawRotation = FMath::FInterpTo(GetControlRotation().Yaw, GetControlRotation().Yaw + YawInputValue, DeltaSeconds, YawRotationSpeed);

	GetController()->SetControlRotation(FRotator(NewPitchRotation, NewYawRotation, 0));
	CameraRootComponent->SetWorldRotation(FRotator(CameraRootComponent->GetComponentRotation().Pitch, NewYawRotation, 0));
}

void APlayerCamera::MoveToRoot(float DeltaSeconds)
{
	FVector CameraLocation = Camera->GetRelativeLocation();
	float NewCameraDistance = FMath::FInterpTo(CameraLocation.X, TargetCameraDistanceFromFocusPoint, DeltaSeconds, CameraToRootSpeed);
	CameraLocation.X = NewCameraDistance;
	Camera->SetRelativeLocation(CameraLocation);
}

void APlayerCamera::Follow(float DeltaSeconds)
{
	if (FollowTarget)
	{
		FVector TargetLocation = FollowTarget->GetActorLocation();
		TargetLocation.Z = GetActorLocation().Z;

		FVector NewCameraLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, FollowSpeed);
		SetActorLocation(NewCameraLocation);

		if (bDisableFollowWhenOnTarget && TargetLocation.Equals(GetActorLocation(), 10.f))
		{
			StopFollowing();
		}
	}
}

void APlayerCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceGround(DeltaSeconds);

	Rotate(DeltaSeconds);

	MoveToRoot(DeltaSeconds);

	Follow(DeltaSeconds);
}

void APlayerCamera::CameraMoveOrthogonalCompleted()
{
	MovementInputSeconds = 0.f;
}

void APlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->AddMappingContext(PlayerCameraMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	/* Input Bindings */

	EnhancedInputComponent->BindAction(CameraInputConfig->CameraMoveVertical, ETriggerEvent::Triggered, this, &APlayerCamera::CameraMoveVertical);
	EnhancedInputComponent->BindAction(CameraInputConfig->CameraMoveOrthogonal, ETriggerEvent::Triggered, this, &APlayerCamera::CameraMoveOrthogonal);
	EnhancedInputComponent->BindAction(CameraInputConfig->CameraMoveOrthogonal, ETriggerEvent::Completed, this,
									   &APlayerCamera::CameraMoveOrthogonalCompleted);
	EnhancedInputComponent->BindAction(CameraInputConfig->CameraRotateYaw, ETriggerEvent::Triggered, this, &APlayerCamera::CameraRotateYaw);
	EnhancedInputComponent->BindAction(CameraInputConfig->AllowCameraRotate, ETriggerEvent::Completed, this, &APlayerCamera::StopRotatingYaw);
}

void APlayerCamera::StopRotatingYaw()
{
	YawInputValue = 0.f;
}

void APlayerCamera::CameraMoveVertical(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		const float InputValue = InputActionValue.Get<float>();

		float CameraVerticalMovementStep = (MaxCameraHeight - MinCameraHeight) / CameraVerticalMovementStepCount;

		CameraHeight = FMath::Clamp(CameraHeight + CameraVerticalMovementStep * -InputValue, MinCameraHeight, MaxCameraHeight);

		TargetCameraPitchRotation = CalculateAlphaFromCameraGroundDistance(MinCameraPitchRotation, MaxCameraPitchRotation);
		TargetCameraDistanceFromFocusPoint = CalculateAlphaFromCameraGroundDistance(MinCameraDistanceFromFocusPoint, MaxCameraDistanceFromFocusPoint);
	}
}

void APlayerCamera::CameraMoveOrthogonal(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0, GetActorRotation().Yaw, 0);

		if (MoveValue.Y != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);

			AddMovementInput(Direction, MoveValue.Y);
		}

		if (MoveValue.X != 0.f)
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

			AddMovementInput(Direction, MoveValue.X);
		}

		if (FollowTarget)
		{
			MovementInputSeconds += GetWorld()->GetDeltaSeconds();
			if (MovementInputSeconds > DisableFollowTimeSeconds)
			{
				StopFollowing();
			}
		}
	}
}

void APlayerCamera::CameraRotateYaw(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		YawInputValue = InputActionValue.Get<float>();
	}
}
