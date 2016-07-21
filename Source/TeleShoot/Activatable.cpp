// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Activatable.h"


// Sets default values
AActivatable::AActivatable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActivatable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AActivatable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActivatable::Activate()
{
}

void AActivatable::Deactivate()
{

}

