// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Crystal.generated.h"

UCLASS()
class TELESHOOT_API ACrystal : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UCapsuleComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

public:
	// Sets default values for this actor's properties
	ACrystal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE class UCapsuleComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UStaticMeshComponent* GetProjectileMovement() const { return MeshComponent; }

	void MoveActorTo(FVector NewLocation);

private:
	FVector NewLocation;
	bool HasNewLocation;
};
