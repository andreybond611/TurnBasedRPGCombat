// Copyright 2022 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/RPGTeams.h"
#include "AI/Pathfinding/DynamicObstacle.h"
#include "Interaction/Damageable.h"
#include "Interaction/GameplayTagHolder.h"
#include "Interaction/Outlinable.h"
#include "TurnBased/TurnBasedBattleParticipant.h"
#include "Utility/TurnBasedTypes.h"
#include "RPGCharacter.generated.h"

class UPathFollowingComponent;
class UNavigationPath;
class URPGAnimInstance;
class UWidgetComponent;
class UNavArea;
class UNavModifierComponent;
class UEffectComponent;
class UEffect;
class UAbilityComponent;
class UAbility;
class UStatsComponent;
class UStatsComponent;
class ATurnBasedBattle;
class UAbilitySystemComponent;
class UNiagaraComponent;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FRotationState
{
	GENERATED_BODY()
public:
	FRotationState() = default;

	bool bIsRotating = false;
	UPROPERTY()
	AActor* Actor = nullptr;
	FVector Location = FVector::ZeroVector;
	float Speed = 0.f;
	bool bStopWhenComplete = false;
};

class UTurnBasedComponent;
class ARPGCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotationEnded);
DECLARE_DELEGATE(FOnHitLanded);
DECLARE_DELEGATE(FOnProjectileSpawn);

UCLASS(BlueprintType, Blueprintable)
class TURNBASEDRPGCOMBAT_API ARPGCharacter : public ACharacter,
											 public IGenericTeamAgentInterface,
											 public IGameplayTagHolder,
											 public ITBBattleParticipant,
											 public IDynamicObstacle,
											 public IOutlinable,
											 public IDamageable
{
	GENERATED_BODY()

public:
	ARPGCharacter(const FObjectInitializer& ObjectInitializer);
	UPathFollowingComponent* FindPathFollowingComponent();

	UFUNCTION(BlueprintCallable)
	void MoveToLocation(const FVector& TargetLocation);
	void MoveToActor(AActor* Actor, float MinDistanceToActor);

	bool IsInBattle() const { return CurrentBattle != nullptr; }

	/**
	 * Rotates character to face TargetLocation
	 * @param TargetLocation - location to face
	 * @param RotationSpeed - speed of rotation
	 * @param bStopWhenFaceTarget - if true, then rotates until this character faces the target,
	 * if false, then always tries to face TargetLocation until StopRotating() is called
	 */
	UFUNCTION(BlueprintCallable)
	void RotateToFaceLocation(const FVector& TargetLocation, float RotationSpeed = 500.f, bool bStopWhenFaceTarget = true);
	UFUNCTION(BlueprintCallable)
	void RotateToFaceActor(AActor* TargetActor, float RotationSpeed = 500.f, bool bStopWhenFaceTarget = true);
	UFUNCTION(BlueprintCallable)
	void StopRotating();

	UFUNCTION(BlueprintCallable)
	void ReadyAbility(int32 Index);

	UFUNCTION(BlueprintCallable)
	void UnReadyCurrentAbility();

	UFUNCTION(BlueprintCallable)
	void GiveControlToPlayer();

	UFUNCTION(BlueprintCallable)
	void RemoveControlFromPlayer();

	void ExecuteReadyAbility();

	UFUNCTION(BlueprintCallable)
	void ApplyEffect(UEffect* Effect);

	UFUNCTION(BlueprintCallable)
	void FocusCamera();

	int32 CalculatePathAPCost(UNavigationPath* NavigationPath);
	int32 CalculatePathAPCost(float NavigationPathLength);
	float CalculateHitChance(const AActor* Target);

	void Die();

	UPROPERTY(BlueprintAssignable)
	FOnRotationEnded OnRotationEnded;

	FOnHitLanded OnHitLanded;
	FOnProjectileSpawn OnProjectileSpawn;

	void MeleeAttackLanded(ARPGCharacter* Target);

	void ConsumeActionPoints(int32 Points);
	void ConsumeActionPoint();

	bool GetIsPlayableAtStart() const { return bPlayableAtGameStart; }

	float GetMaxWalkLength() const;
	float GetCurrentWalkLength() const { return WalkLengthPerTurn; }

	UParticleSystemComponent* GetParticleSystemComponent() const { return ParticleSystemComponent; }
	UNiagaraComponent* GetBodyEffectComponent() const { return BodyEffect; }
	UChildActorComponent* GetMagicCircleComponent() const { return MagicCircleComponent; }
	UAbilityComponent* GetAbilityComponent() const { return AbilityComponent; }
	USceneComponent* GetProjectileStartComponent() const { return ProjectileStartComponent; }
	UEffectComponent* GetEffectComponent() const { return EffectComponent; }

	bool IsTurnAllowed();
	bool IsMoving();
	float GetPassedDistanceSinceLastFrame() const { return PassedDistanceSinceLastFrame; }

	/* IGenericTeamAgentInterface begin */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* IGenericTeamAgentInterface end */

	/* IGameplayTagAssetInterface begin */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	/* IGameplayTagAssetInterface end */

	/* ITurnBasedBattleParticipant begin */
	virtual UTurnBasedComponent* GetTurnBasedComponent() const override;
	virtual UStatsComponent* Stats() const override;
	virtual UTexture2D* GetPortrait() const override;
	virtual bool IsPlayerControlled() const override;
	/* ITurnBasedBattleParticipant end */

	/* IDynamicObstacle begin*/
	virtual void EnableDynamicObstacle() override;
	virtual void DisableDynamicObstacle() override;
	/* IDynamicObstacle end*/

	/* IOutlinable begin */
	virtual void EnableOutline(EOutlineColor Color) override;
	virtual void DisableOutline() override;
	/* IOutlinable end */

	/* IDamageable begin */
	virtual void GetDamaged(const FDamage& Damage) override;
	/* IDamageable end */

	void AddTag(const FGameplayTag& Tag);
	void AddTags(const FGameplayTagContainer& GameplayTags);
	void RemoveTag(const FGameplayTag& Tag);
	void RemoveTags(const FGameplayTagContainer& GameplayTags);

	UFUNCTION(BlueprintPure)
	bool HasTag(const FGameplayTag& Tag) const;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	TSubclassOf<ARPGCharacter> CharacterBaseBPClass;
	UPROPERTY(EditAnywhere)
	TArray<FName> StatsToRemove;
#endif


protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TurnBased", meta = (AllowPrivateAccess = "true"))
	UTurnBasedComponent* TurnBasedComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAbilityComponent* AbilityComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UEffectComponent* EffectComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* FloatingDamageWidgetComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* BodyEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* MagicCircleComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileStartComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CharacterFeet;

	FVector CachedLocation;
	float WalkLengthPerTurn = 0.f;
	float AvailableDistanceForConsumedPoint = 0.f;
	float WalkedDistanceSinceStartTurn = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player settings", meta = (AllowPrivateAccess = "true"))
	bool bPlayableAtGameStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (AllowPrivateAccess = "true"))
	UTexture2D* CharacterPortraitImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitFront;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitBack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	UStatsComponent* StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team", meta = (AllowPrivateAccess = "true"))
	ERPGTeam Team;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotationState RotationState;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bPlayerControlled = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bFromPlayersParty = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCurrentTurn = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ATurnBasedBattle* CurrentBattle;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer GameplayTagContainer;

	void Rotate(float DeltaTime);
	float CalculateAverageMovePerFrame(float FrameMove);
	void Move();

	// To start turn call TurnBasedComponent->StartTurn()
	UFUNCTION()
	void OnStartTurn();
	// To end turn call TurnBasedComponent->EndTurn()
	UFUNCTION()
	void OnEndTurn();

	UPROPERTY()
	TArray<float> FrameMoves;
	float AverageMovePerFrame = 0.f;
	float PassedDistanceSinceLastFrame;

	UPROPERTY()
	URPGAnimInstance* AnimInstance;

	void PlayHitAnimation(EHitDirection HitDirection);
#if WITH_EDITOR
	void AddMissingStats();
#endif
};
