// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "Abilities/Projectiles/BoulderProjectile.h"

void ABoulderProjectile::EmergeFromGround_Implementation()
{

}

void ABoulderProjectile::BeginPlay()
{
	Super::BeginPlay();

	EmergeFromGround();
}

void ABoulderProjectile::ProjectileHit()
{
	Super::ProjectileHit();
}
