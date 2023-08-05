// Copyright 2022 Andrei Bondarenko. All rights reserved

#include "Characters/RPGCharacter.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "ActorComponents/TurnBasedComponent.h"
#include "Algo/Accumulate.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/AbilityComponent.h"
#include "AI/TurnBasedAIController.h"
#include "CharacterProgression/StatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UnrealFramework/RPGPlayerController.h"
#include "Utility/TurnBasedUtility.h"
#include "NiagaraComponent.h"
#include "Abilities/Ability.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Abilities/Effects/EffectComponent.h"
#include "ActorComponents/RPGMovementComponent.h"
#include "CharacterProgression/GameStat.h"
#include "Characters/PlayerCamera.h"
#include "Components/BoxComponent.h"
#include "NavAreas/NavArea_Default.h"
#include "NavAreas/NavArea_Null.h"
#include "UI/DamageNumbersWidget.h"

DEFINE_LOG_CATEGORY_STATIC(RPGCharacter, All, All);

ARPGCharacter::ARPGCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URPGMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	TurnBasedComponent = CreateDefaultSubobject<UTurnBasedComponent>("TurnBasedComponent");
	StatsComponent = CreateDefaultSubobject<UStatsComponent>("Stats");
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>("AbilityComponent");
	EffectComponent = CreateDefaultSubobject<UEffectComponent>("EffectComponent");
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	BodyEffect = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	MagicCircleComponent = CreateDefaultSubobject<UChildActorComponent>("MagicCircle");
	FloatingDamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("FloatingDamageNumbers");
	ProjectileStartComponent = CreateDefaultSubobject<USceneComponent>("ProjectileStart");
	CharacterFeet = CreateDefaultSubobject<UBoxComponent>("CharacterFeet");

	ParticleSystemComponent->SetupAttachment(GetMesh());
	BodyEffect->SetupAttachment(GetMesh());
	MagicCircleComponent->SetupAttachment(GetMesh());
	FloatingDamageWidgetComponent->SetupAttachment(GetCapsuleComponent());
	ProjectileStartComponent->SetupAttachment(GetCapsuleComponent());
	CharacterFeet->SetupAttachment(GetCapsuleComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCapsuleRadius(34.f);
	GetCapsuleComponent()->SetCanEverAffectNavigation(true);
	GetCapsuleComponent()->SetAreaClassOverride(UNavArea_Null::StaticClass());
}

UPathFollowingComponent* ARPGCharacter::FindPathFollowingComponent()
{
	UPathFollowingComponent* PathFollowingComponent = nullptr;
	PathFollowingComponent = FindComponentByClass<UPathFollowingComponent>();
	if (PathFollowingComponent == nullptr)
	{
		PathFollowingComponent = NewObject<UPathFollowingComponent>(this);
		PathFollowingComponent->RegisterComponentWithWorld(GetWorld());
		PathFollowingComponent->Initialize();
	}
	return PathFollowingComponent;
}

void ARPGCharacter::MoveToLocation(const FVector& TargetLocation)
{
	UPathFollowingComponent* PathFollowingComponent = FindPathFollowingComponent();
	if (PathFollowingComponent)
	{
		FAIMoveRequest MoveRequest(TargetLocation);
		MoveRequest.SetReachTestIncludesAgentRadius(false);

		UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetLocation(), TargetLocation);
		PathFollowingComponent->RequestMove(MoveRequest, NavigationPath->GetPath());
	}
}

void ARPGCharacter::MoveToActor(AActor* Actor, float MinDistanceToActor)
{
	UPathFollowingComponent* PathFollowingComponent = FindPathFollowingComponent();
	if (PathFollowingComponent)
	{
		UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetLocation(), Actor->GetActorLocation());

		FVector SearchedLocation = FVector::Zero();

		if (NavigationPath->GetPath().IsValid())
		{
			if (NavigationPath->GetPathLength() > MinDistanceToActor)
			{
				SearchedLocation = UTurnBasedUtility::GetLocationOnPathNearActor(NavigationPath, MinDistanceToActor);
			}
			else
			{
				PathFollowingComponent->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
				RotateToFaceLocation(Actor->GetActorLocation());
				return;
			}
		}
		FAIMoveRequest MoveRequest(SearchedLocation);
		MoveRequest.SetReachTestIncludesAgentRadius(false);
		NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetLocation(), SearchedLocation);
		PathFollowingComponent->RequestMove(MoveRequest, NavigationPath->GetPath());
	}
}

void ARPGCharacter::PlayHitAnimation(EHitDirection HitDirection)
{
	switch (HitDirection)
	{
	case EHitDirection::None: break;
	case EHitDirection::Front: PlayAnimMontage(HitFront); break;
	case EHitDirection::Right: PlayAnimMontage(HitRight); break;
	case EHitDirection::Back: PlayAnimMontage(HitLeft); break;
	case EHitDirection::Left: PlayAnimMontage(HitBack); break;
	default: checkNoEntry();
	}
}

void ARPGCharacter::RotateToFaceLocation(const FVector& TargetLocation, float RotationSpeed, bool bStopWhenFaceTarget)
{
	RotationState = {true, nullptr, TargetLocation, RotationSpeed, bStopWhenFaceTarget};
}

void ARPGCharacter::RotateToFaceActor(AActor* TargetActor, float RotationSpeed, bool bStopWhenFaceTarget)
{
	RotationState = {true, TargetActor, {}, RotationSpeed, bStopWhenFaceTarget};
}

void ARPGCharacter::StopRotating()
{
	RotationState = FRotationState();
	OnRotationEnded.Broadcast();
}

void ARPGCharacter::EnableOutline(EOutlineColor Color)
{
	int32 Stencil = static_cast<int32>(Color) + 1;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue = Stencil;
}

void ARPGCharacter::DisableOutline()
{
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->CustomDepthStencilValue = 0;
}

void ARPGCharacter::GetDamaged(const FDamage& Damage)
{
	UDamageNumbersWidget* DamageNumbersWidget = Cast<UDamageNumbersWidget>(FloatingDamageWidgetComponent->GetUserWidgetObject());
	check(DamageNumbersWidget);

	float HealthDamage = 0;

	EDamageColor DamageColor;
	FName ArmorStatName;
	if (Damage.DamageType == EDamageType::Physical)
	{
		ArmorStatName = SN_PhysicalArmor;
		DamageColor = EDamageColor::PhysicalArmor;
	}
	else
	{
		ArmorStatName = SN_MagicArmor;
		DamageColor = EDamageColor::MagicArmor;
	}

	float Armor = Stats()->Get(ArmorStatName);
	if (Armor >= Damage.DamageNumber)
	{
		Stats()->Remove(ArmorStatName, Damage.DamageNumber);
		DamageNumbersWidget->AddDamageNumber(Damage.DamageNumber, DamageColor);
	}
	else
	{
		if (Armor > 0.f)
		{
			Stats()->Remove(ArmorStatName, Armor);
			DamageNumbersWidget->AddDamageNumber(Armor, DamageColor);
			HealthDamage = -(Armor - Damage.DamageNumber);
		}
		else
		{
			HealthDamage = Damage.DamageNumber;
		}
	}

	if (HealthDamage > 0.f)
	{
		Stats()->Remove(SN_Health, HealthDamage);
		DamageNumbersWidget->AddDamageNumber(HealthDamage, static_cast<EDamageColor>(Damage.DamageType));
	}

	PlayHitAnimation(Damage.HitDirection);
}

bool ARPGCharacter::IsTurnAllowed()
{
	FGameplayTag StunTag = FGameplayTag::RequestGameplayTag("Effect.Debuff.Stun");
	bool bStunt = HasTag(StunTag);

	return !bStunt;
}

void ARPGCharacter::FocusCamera()
{
	auto* PlayerController = GetWorld()->GetFirstPlayerController<ARPGPlayerController>();
	check(PlayerController)

		auto* PlayerCamera = PlayerController->GetPawn<APlayerCamera>();
	check(PlayerCamera)

		PlayerCamera->StartFollowing(this);
}

void ARPGCharacter::OnStartTurn()
{
	FocusCamera();
	DisableDynamicObstacle();

	if (!IsTurnAllowed())
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ARPGCharacter::OnEndTurn, 1.f, false);

		return;
	}

	Stats()->Set(SN_ActionPoints, Stats()->Get(SN_MaxActionPoints));
	WalkLengthPerTurn = GetMaxWalkLength();
	WalkedDistanceSinceStartTurn = 0.f;
	AvailableDistanceForConsumedPoint = 0.f;

	if (bPlayerControlled)
	{
		auto PlayerController = GetWorld()->GetFirstPlayerController<ARPGPlayerController>();
		checkf(PlayerController, TEXT("PlayerController is null"));

		PlayerController->SetControlledCharacter(this);
	}

	EffectComponent->StartTurn();
}

void ARPGCharacter::OnEndTurn()
{
	GetCharacterMovement()->StopActiveMovement();
	EnableDynamicObstacle();

	if (bPlayerControlled)
	{
		auto PlayerController = GetWorld()->GetFirstPlayerController<ARPGPlayerController>();
		PlayerController->SetControlledCharacter(nullptr);
	}

	FrameMoves.Empty();
}

int32 ARPGCharacter::CalculatePathAPCost(UNavigationPath* NavigationPath)
{
	if (!NavigationPath->GetPath().IsValid())
	{
		return 0;
	}

	float PathLength = NavigationPath->GetPathLength();

	if (PathLength < AvailableDistanceForConsumedPoint)
	{
		return 0;
	}
	return CalculatePathAPCost(PathLength);
}

int32 ARPGCharacter::CalculatePathAPCost(float NavigationPathLength)
{
	return FMath::CeilToInt32((NavigationPathLength - AvailableDistanceForConsumedPoint) / Stats()->Get(SN_MaxWalkPerAP));
}

float ARPGCharacter::CalculateHitChance(const AActor* Target)
{
	if (auto OtherStats = Target->FindComponentByClass<UStatsComponent>())
	{
		return Stats()->Get(SN_Accuracy) - OtherStats->Get(SN_Dodge);
	}
	return 0.f;
}

void ARPGCharacter::Die()
{
	PlayAnimMontage(DeathAnimation);
}

void ARPGCharacter::MeleeAttackLanded(ARPGCharacter* Target)
{
	float DamageNumber = Stats()->Get(SN_Damage);
	EHitDirection HitDirection = UTurnBasedUtility::FindHitDirection(this, Target);

	Target->GetDamaged({DamageNumber, HitDirection});
}

void ARPGCharacter::ConsumeActionPoints(int32 Points)
{
	Stats()->Remove(SN_ActionPoints, Points);
	if (FMath::RoundToInt32(Stats()->Get(SN_ActionPoints)) == 0 && GetTurnBasedComponent()->IsCurrentTurn())
	{
		TurnBasedComponent->EndTurn();
	}
}

void ARPGCharacter::ConsumeActionPoint()
{
	ConsumeActionPoints(1);
}

float ARPGCharacter::GetMaxWalkLength() const
{
	return Stats()->Get(SN_ActionPoints) * Stats()->Get(SN_MaxWalkPerAP);
}

void ARPGCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	Team = static_cast<ERPGTeam>(TeamID.GetId());
}

FGenericTeamId ARPGCharacter::GetGenericTeamId() const
{
	return static_cast<uint8>(Team);
}

void ARPGCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}

UTurnBasedComponent* ARPGCharacter::GetTurnBasedComponent() const
{
	return TurnBasedComponent;
}

UStatsComponent* ARPGCharacter::Stats() const
{
	return StatsComponent;
}

UTexture2D* ARPGCharacter::GetPortrait() const
{
	return CharacterPortraitImage;
}

bool ARPGCharacter::IsPlayerControlled() const
{
	return bPlayerControlled;
}

void ARPGCharacter::EnableDynamicObstacle()
{
	GetCapsuleComponent()->SetAreaClassOverride(UNavArea_Null::StaticClass());
	GetCapsuleComponent()->SetCanEverAffectNavigation(true);
}

void ARPGCharacter::DisableDynamicObstacle()
{
	GetCapsuleComponent()->SetAreaClassOverride(UNavArea_Default::StaticClass());
	GetCapsuleComponent()->SetCanEverAffectNavigation(false);
}

void ARPGCharacter::AddTag(const FGameplayTag& Tag)
{
	GameplayTagContainer.AddTag(Tag);
	AnimInstance->OnGameplayTagsChanged.Broadcast(GameplayTagContainer);
}

void ARPGCharacter::AddTags(const FGameplayTagContainer& GameplayTags)
{
	GameplayTagContainer.AppendTags(GameplayTags);
}

void ARPGCharacter::RemoveTag(const FGameplayTag& Tag)
{
	GameplayTagContainer.RemoveTag(Tag);
	AnimInstance->OnGameplayTagsChanged.Broadcast(GameplayTagContainer);
}

void ARPGCharacter::RemoveTags(const FGameplayTagContainer& GameplayTags)
{
	GameplayTagContainer.RemoveTags(GameplayTags);
}

bool ARPGCharacter::HasTag(const FGameplayTag& Tag) const
{
	return GameplayTagContainer.HasTag(Tag);
}

void ARPGCharacter::ReadyAbility(int32 Index)
{
	AbilityComponent->ReadyAbilityAt(Index);
}

void ARPGCharacter::UnReadyCurrentAbility()
{
	AbilityComponent->CancelReadyAbility();
}

void ARPGCharacter::GiveControlToPlayer()
{
	bPlayerControlled = true;
	auto* AIController = GetController<ATurnBasedAIController>();
	if (ensure(AIController))
	{
		AIController->SetPlayerControlled(true);
	}
}

void ARPGCharacter::RemoveControlFromPlayer()
{
	bPlayerControlled = false;
	auto* AIController = GetController<ATurnBasedAIController>();
	if (ensure(AIController))
	{
		AIController->SetPlayerControlled(false);
	}
}

void ARPGCharacter::ExecuteReadyAbility()
{
	AbilityComponent->ExecuteReadyAbility();
}

void ARPGCharacter::ApplyEffect(UEffect* Effect)
{
	EffectComponent->AddEffect(Effect);
}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// clothing simulation is broken in UE5 with the paragon characters
	GetMesh()->SuspendClothingSimulation();

	if (CachedLocation == FVector::ZeroVector)
	{
		CachedLocation = GetLocation();
	}

	AnimInstance = Cast<URPGAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance)

		StatsComponent->FindStat(SN_Health)
			->OnReachMinValue.AddUObject(this, &ARPGCharacter::Die);

	GetCapsuleComponent()->SetCanEverAffectNavigation(true);
}

void ARPGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TurnBasedComponent->OnTurnBegin.AddDynamic(this, &ARPGCharacter::OnStartTurn);
	TurnBasedComponent->OnTurnEnd.AddDynamic(this, &ARPGCharacter::OnEndTurn);
}

void ARPGCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ARPGCharacter::Rotate(float DeltaTime)
{
	if (RotationState.bIsRotating)
	{
		FVector TargetLocation = RotationState.Actor ? RotationState.Actor->GetActorLocation() : RotationState.Location;
		const FVector DiffVector = TargetLocation - GetLocation();
		FRotator TargetRotation = FRotator(0.f, DiffVector.Rotation().Yaw, 0.f);

		const FRotator NextRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, RotationState.Speed);
		SetActorRotation(NextRotation);
		if (RotationState.bStopWhenComplete && GetActorRotation().Equals(TargetRotation, 0.001f))
		{
			StopRotating();
		}
	}
}

float ARPGCharacter::CalculateAverageMovePerFrame(float FrameMove)
{
	if (FrameMove != 0.f)
	{
		FrameMoves.Add(FrameMove);
	}
	if (!FrameMoves.IsEmpty())
	{
		return Algo::Accumulate(FrameMoves, 0.f) / FrameMoves.Num();
	}
	return 0.f;
}

bool ARPGCharacter::IsMoving()
{
	return GetCharacterMovement()->GetLastUpdateVelocity() != FVector::ZeroVector;
}

void ARPGCharacter::Move()
{
	FVector CurrentLocation = GetLocation();
	PassedDistanceSinceLastFrame = FVector::Dist(CachedLocation, CurrentLocation);
	CachedLocation = CurrentLocation;

	if (GetTurnBasedComponent()->IsCurrentTurn() && IsMoving())
	{
		WalkLengthPerTurn -= PassedDistanceSinceLastFrame;
		if (WalkLengthPerTurn <= AverageMovePerFrame)
		{
			WalkLengthPerTurn = 0.f;
			GetMovementComponent()->StopMovementImmediately();
		}

		AvailableDistanceForConsumedPoint -= PassedDistanceSinceLastFrame;
		if (AvailableDistanceForConsumedPoint < 0.f)
		{
			ConsumeActionPoint();
			AvailableDistanceForConsumedPoint += Stats()->Get(SN_MaxWalkPerAP);
		}

		WalkedDistanceSinceStartTurn += PassedDistanceSinceLastFrame;

		AverageMovePerFrame = CalculateAverageMovePerFrame(PassedDistanceSinceLastFrame);
	}
}

void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();

	Rotate(DeltaTime);
}
