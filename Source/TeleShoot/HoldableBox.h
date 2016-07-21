// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HoldableBox.generated.h"

UCLASS()
class TELESHOOT_API AHoldableBox : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;


public:	
	// Sets default values for this actor's properties
	AHoldableBox();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void PickUp();

	void Drop(FVector Location, FRotator Rotation);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Opens)
		FVector EndLocation;

private:
	float CurrentY;
	
};
