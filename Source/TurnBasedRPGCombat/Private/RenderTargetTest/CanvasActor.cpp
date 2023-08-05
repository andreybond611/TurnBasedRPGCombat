// Copyright 2022 - 2023 Andrei Bondarenko. All rights reserved


#include "RenderTargetTest/CanvasActor.h"

// Sets default values
ACanvasActor::ACanvasActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACanvasActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACanvasActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

