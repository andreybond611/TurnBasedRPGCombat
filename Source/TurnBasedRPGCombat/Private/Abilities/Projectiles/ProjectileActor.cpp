// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved

#include "Abilities/Projectiles/ProjectileActor.h"

#include "Characters/RPGCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

void AProjectileActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnProjectileHitDelegate.ExecuteIfBound(OtherActor, GetActorLocation());
	ProjectileHit();
}

void AProjectileActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (auto TeamAgent = Cast<IGenericTeamAgentInterface>(ProjectileOwner))
	{
		if (TeamAgent->GetTeamAttitudeTowards(*OtherActor) == ETeamAttitude::Hostile)
		{
			OnProjectileHitDelegate.ExecuteIfBound(OtherActor, GetActorLocation());
			ProjectileHit();
		}
	}
}

AProjectileActor::AProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere");

	SetRootComponent(SphereCollision);
	Mesh->SetupAttachment(SphereCollision);

	SphereCollision->OnComponentHit.AddDynamic(this, &AProjectileActor::OnHit);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnOverlap);
}

void AProjectileActor::SetProjectileOwner(AActor* InProjectileOwner)
{
	ProjectileOwner = InProjectileOwner;
}

void AProjectileActor::ProjectileHit()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, GetActorLocation());
	Destroy();
}
