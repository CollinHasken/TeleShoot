// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Activatable.h"
#include "Lever.generated.h"

UCLASS()
class TELESHOOT_API ALever : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

public:
	// Sets default values for this actor's properties
	ALever();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void Switch();

	UPROPERTY(EditAnywhere, Category = Activates)
	TArray<AActivatable*> Activatable;

	bool Activated;
};
