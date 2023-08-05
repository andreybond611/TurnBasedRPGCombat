// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/MeleeAttackAbility.h"

#include "Abilities/AbilityComponent.h"
#include "Abilities/MoveAbility.h"
#include "Abilities/TargetTypes/AbilityTargetState.h"
#include "CharacterProgression/StatsComponent.h"
#include "Characters/RPGCharacter.h"
#include "Utility/TurnBasedUtility.h"

void UMeleeAttackAbility::AddToCharacter(ARPGCharacter* Character)
{
	Super::AddToCharacter(Character);

	UAbilityComponent* AbilityComponent = Character->GetAbilityComponent();
	MoveAbility = AbilityComponent->GetMoveAbility();
}

void UMeleeAttackAbility::StartAbility()
{
	if (GetTarget().Actor)
	{
		MoveForMeleeAttack(GetTarget().Actor);
	}
	else
	{
		OnRequestFinishedHandle =
			Owner->FindPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UMeleeAttackAbility::OnMoveToLocationFinished);

		MoveAbility->GetTargetState()->SetTargetLocation(GetTarget().Location);
		MoveAbility->SetAcceptanceRadius(GetRange());
		MoveAbility->StartAbility();
	}
}

void UMeleeAttackAbility::MoveForMeleeAttack(AActor* TargetActor)
{
	OnRequestFinishedHandle = Owner->FindPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UMeleeAttackAbility::OnMoveToActorFinished);
	MoveAbility->GetTargetState()->SetTargetActor(TargetActor);
	MoveAbility->SetAcceptanceRadius(GetRange());
	MoveAbility->StartAbility();

	MeleeTarget = TargetActor;
}

void UMeleeAttackAbility::EndAbility()
{
	Super::EndAbility();
}

void UMeleeAttackAbility::MeleeAttackLanded(AActor* TargetActor)
{
	if (auto Damageable = Cast<IDamageable>(TargetActor))
	{
		float DamageNumber = Owner->Stats()->Get(SN_Damage);
		EHitDirection HitDirection = UTurnBasedUtility::FindHitDirection(Owner, TargetActor);

		FDamage Damage;
		Damage.DamageNumber = DamageNumber;
		Damage.HitDirection = HitDirection;
		Damage.DamageType = DamageType;

		Damageable->GetDamaged(Damage);
	}
}

void UMeleeAttackAbility::MeleeAttackDone()
{
	Owner->FindPathFollowingComponent()->OnRequestFinished.Remove(OnRequestFinishedHandle);
	EndAbility();
}

void UMeleeAttackAbility::OnMoveToActorFinished(FAIRequestID AIRequestID, const FPathFollowingResult& PathFollowingResult)
{
	if (PathFollowingResult.IsSuccess())
	{
		checkf(MeleeTarget, TEXT("MeleeTarget is null in MeleeAttackAbility with owner %s"), *Owner->GetName())

		Owner->RotateToFaceLocation(MeleeTarget->GetActorLocation());

		checkf(!MeleeAttackMontages.IsEmpty(), TEXT("%s doesn't have melee attack anim montages"), *GetName());

		int32 MontagesNum = MeleeAttackMontages.Num() - 1;
		UAnimMontage* MeleeMontage = MeleeAttackMontages[FMath::RandRange(0, MontagesNum)];
		float AttackMontageLength = Owner->PlayAnimMontage(MeleeMontage);

		Owner->OnHitLanded.BindUObject(this, &UMeleeAttackAbility::MeleeAttackLanded, MeleeTarget);

		if (AttackMontageLength > 0.f)
		{
			FTimerHandle AttackMontageTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(AttackMontageTimerHandle, this, &UMeleeAttackAbility::MeleeAttackDone, AttackMontageLength, false);
		}
		return;
	}
	MeleeAttackDone();
}

void UMeleeAttackAbility::OnMoveToLocationFinished(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult)
{
	Owner->FindPathFollowingComponent()->OnRequestFinished.Remove(OnRequestFinishedHandle);
	EndAbility();
}
