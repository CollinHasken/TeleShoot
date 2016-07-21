// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Activatable.generated.h"

UCLASS()
class TELESHOOT_API AActivatable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActivatable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void Activate();
	virtual void Deactivate();
};
