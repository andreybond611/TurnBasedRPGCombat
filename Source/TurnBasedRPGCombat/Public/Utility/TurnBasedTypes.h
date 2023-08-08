// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedTypes.generated.h"

struct FLineSegment
{
	FVector LineStart;
	FVector LineEnd;

	FVector Vector()const
	{
		FVector SegmentVector = LineEnd - LineStart;
		return SegmentVector;
	}
};

UENUM(BlueprintType)
enum class EOutlineColor : uint8
{
	White,
	Green,
	Red,
	Yellow
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	None,
	Front,
	Right,
	Back,
	Left
};

USTRUCT(BlueprintType)
struct FTarget
{
	GENERATED_BODY()
	
	FVector Location = FVector::ZeroVector;
	UPROPERTY()
	AActor* Actor = nullptr;
	UPROPERTY()
	TArray<AActor*> MultipleActors;
};

constexpr float MaxStatValue = 9999.f;
constexpr float MinStatValue = -9999.f;

// Stat names

const FName SN_Health = FName("Health");
const FName SN_MaxHealth = FName("MaxHealth");
const FName SN_ActionPoints = FName("ActionPoints");
const FName SN_MaxActionPoints = FName("MaxActionPoints");
const FName SN_Initiative = FName("Initiative");
const FName SN_MaxWalkPerAP = FName("MaxWalkPerAP");
const FName SN_Dodge = FName("Dodge");
const FName SN_Accuracy = FName("Accuracy");
const FName SN_Damage = FName("Damage");
const FName SN_PhysicalArmor = FName("PhysicalArmor");
const FName SN_MaxPhysicalArmor = FName("MaxPhysicalArmor");
const FName SN_MagicArmor = FName("MagicArmor");
const FName SN_MaxMagicArmor = FName("MaxMagicArmor");

const FName SN_PhysicalResistance = FName("PhysicalResistance");
const FName SN_FireResistance = FName("FireResistance");
const FName SN_EarthResistance = FName("EarthResistance");
const FName SN_PoisonResistance = FName("PoisonResistance");
const FName SN_IceResistance = FName("IceResistance");

/**
 * When adding new damage type, also add a new entry in the same place in EDamageColor
 * and add and configure new color in UFloatingNumbersWidget
 */
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Fire,
	Earth,
	Poison,
	Ice,
};

UENUM(BlueprintType)
enum class EFloatingNumberColor : uint8
{
	Physical,
	Fire,
	Earth,
	Poison,
	Ice,
	PhysicalArmor,
	MagicArmor,
	Heal
};

USTRUCT(BlueprintType)
struct FDamage
{
	GENERATED_BODY()

	FDamage() = default;

	float DamageNumber;
	EHitDirection HitDirection;
	EDamageType DamageType;
};

UENUM(BlueprintType)
enum class EPreferredDifficulty : uint8
{
	Weakest,
	Strongest
};

UENUM(BlueprintType)
enum class EPreferredDistance : uint8
{
	Closest,
	Farthest
};
