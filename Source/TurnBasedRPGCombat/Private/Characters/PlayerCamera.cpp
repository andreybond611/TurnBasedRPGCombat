// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "Characters/PlayerCamera.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Input/CameraInputConfigData.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

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

float APlayerCamera::CalculateAlphaFromCameraGroundDistance(const float MinValue, const float MaxValue)
{
	const float MaxDistance = MaxCameraHeight - MinCameraHeight;
	const float CurrentDistance = CameraHeight - MinCameraHeight;
	const float Alpha = CurrentDistance / MaxDistance;

	return FMath::Lerp(MinValue, MaxValue, Alpha);
}

void APlayerCamera::StartFollowing(AActor* ActorToFollow, const bool bInDisableFollowWhenOnTarget)
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

	AController* OwnController = GetController();
	if (OwnController)
	{
		OwnController->SetControlRotation(FRotator(-50.f, 0.f, 0.f));
	}
}

void APlayerCamera::InterpCameraVerticalMove(const float DeltaSeconds, const float TargetGroundDistance)
{
	const float InterpResult = FMath::FInterpTo(GetActorLocation().Z, TargetGroundDistance, DeltaSeconds, VerticalMovementSpeed);
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

void APlayerCamera::Rotate(const float DeltaSeconds)
{
	const float NewPitchRotation = FMath::FInterpTo(GetControlRotation().Pitch, TargetCameraPitchRotation, DeltaSeconds, PitchRotationSpeed);

	const float NewYawRotation = FMath::FInterpTo(GetControlRotation().Yaw, GetControlRotation().Yaw + YawInputValue, DeltaSeconds, YawRotationSpeed);

	AController* OwnController = GetController();
	if (OwnController)
	{
		OwnController->SetControlRotation(FRotator(NewPitchRotation, NewYawRotation, 0));
	}
	CameraRootComponent->SetWorldRotation(FRotator(CameraRootComponent->GetComponentRotation().Pitch, NewYawRotation, 0));
}

void APlayerCamera::MoveToRoot(const float DeltaSeconds)
{
	FVector CameraLocation = Camera->GetRelativeLocation();
	const float NewCameraDistance = FMath::FInterpTo(CameraLocation.X, TargetCameraDistanceFromFocusPoint, DeltaSeconds, CameraToRootSpeed);
	CameraLocation.X = NewCameraDistance;
	Camera->SetRelativeLocation(CameraLocation);
}

void APlayerCamera::Follow(const float DeltaSeconds)
{
	if (FollowTarget)
	{
		FVector TargetLocation = FollowTarget->GetActorLocation();
		TargetLocation.Z = GetActorLocation().Z;

		const FVector NewCameraLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaSeconds, FollowSpeed);
		SetActorLocation(NewCameraLocation);

		if (bDisableFollowWhenOnTarget && TargetLocation.Equals(GetActorLocation(), 10.f))
		{
			StopFollowing();
		}
	}
}

void APlayerCamera::Tick(const float DeltaSeconds)
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

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->AddMappingContext(PlayerCameraMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

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
	if (Controller)
	{
		const float InputValue = InputActionValue.Get<float>();

		const float CameraVerticalMovementStep = (MaxCameraHeight - MinCameraHeight) / CameraVerticalMovementStepCount;

		CameraHeight = FMath::Clamp(CameraHeight + CameraVerticalMovementStep * -InputValue, MinCameraHeight, MaxCameraHeight);

		TargetCameraPitchRotation = CalculateAlphaFromCameraGroundDistance(MinCameraPitchRotation, MaxCameraPitchRotation);
		TargetCameraDistanceFromFocusPoint = CalculateAlphaFromCameraGroundDistance(MinCameraDistanceFromFocusPoint, MaxCameraDistanceFromFocusPoint);
	}
}

void APlayerCamera::DisableFollowWhenMoved()
{
	if (FollowTarget)
	{
		MovementInputSeconds += GetWorld()->GetDeltaSeconds();
		if (MovementInputSeconds > DisableFollowTimeSeconds)
		{
			StopFollowing();
		}
	}
}

void APlayerCamera::CameraMoveOrthogonal(const FInputActionValue& InputActionValue)
{
	if (Controller)
	{
		const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0, GetActorRotation().Yaw, 0);

		if (!FMath::IsNearlyZero(MoveValue.Y))
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);

			AddMovementInput(Direction, MoveValue.Y);
		}

		if (!FMath::IsNearlyZero(MoveValue.X))
		{
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

			AddMovementInput(Direction, MoveValue.X);
		}

		DisableFollowWhenMoved();
	}
}

void APlayerCamera::CameraRotateYaw(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		YawInputValue = InputActionValue.Get<float>();
	}
}
