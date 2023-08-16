// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Characters/RPGCharacter.h"
#include "PlayerCamera.generated.h"

class UFloatingPawnMovement;
class UCameraInputConfigData;
class USphereComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
/**
 * Free camera controlled by the player
 */
UCLASS(BlueprintType, Blueprintable)
class TURNBASEDRPGCOMBAT_API APlayerCamera : public APawn
{
	GENERATED_BODY()
public:
	APlayerCamera();

	/**
	 * Follows an actor until StopFollowing is called
	 * @param bInDisableFollowWhenOnTarget - if true, then stops following when the camera is directly on a target
	 */
	void StartFollowing(AActor* ActorToFollow, bool bInDisableFollowWhenOnTarget = false);
	void StopFollowing();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* PawnMovementComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* CameraRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float MaxCameraHeight = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float MinCameraHeight = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (AllowPrivateAccess = "true"))
	float MinCameraPitchRotation = -20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (AllowPrivateAccess = "true"))
	float MaxCameraPitchRotation = -50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (AllowPrivateAccess = "true"))
	float MinCameraDistanceFromFocusPoint = -300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (AllowPrivateAccess = "true"))
	float MaxCameraDistanceFromFocusPoint = -800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "2", AllowPrivateAccess = "true"))
	float CameraVerticalMovementStepCount = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float CameraOrthogonalMovementSpeed = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float VerticalMovementSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float YawRotationSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float PitchRotationSpeed = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float MaxCameraHeightTraceDistance = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float CameraToRootSpeed = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float FollowSpeed = 2.f;

	/** Disables camera following if player hits moving keys for this time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = "0.1", AllowPrivateAccess = "true"))
	float DisableFollowTimeSeconds = 0.15f;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Camera Values", meta = (AllowPrivateAccess = "true"))
	float CameraHeight = 1000.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Camera Values", meta = (AllowPrivateAccess = "true"))
	float CameraYawRotation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Camera Values", meta = (AllowPrivateAccess = "true"))
	float TargetCameraPitchRotation = -50.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Camera Values", meta = (AllowPrivateAccess = "true"))
	float TargetCameraDistanceFromFocusPoint = -800.f;
	

	// Input

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerCameraMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UCameraInputConfigData* CameraInputConfig;

	// Cached variables

	float YawInputValue;
	UPROPERTY()
	AActor* FollowTarget;
	bool bDisableFollowWhenOnTarget;
	float MovementInputSeconds = 0.f;

	// Input callbacks

	void CameraMoveVertical(const FInputActionValue& InputActionValue);
	void CameraMoveOrthogonal(const FInputActionValue& InputActionValue);
	void CameraRotateYaw(const FInputActionValue& InputActionValue);
	void StopRotatingYaw();
	void CameraMoveOrthogonalCompleted();

	// Private member functions

	float CalculateAlphaFromCameraGroundDistance(float MinValue, float MaxValue);
	void InterpCameraVerticalMove(float DeltaSeconds, float TargetGroundDistance);
	void TraceGround(float DeltaSeconds);
	void Rotate(float DeltaSeconds);
	void MoveToRoot(float DeltaSeconds);
	void Follow(float DeltaSeconds);
	void DisableFollowWhenMoved();
};
