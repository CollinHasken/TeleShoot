// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Orb.generated.h"

UCLASS()
class TELESHOOT_API AOrb : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ActiveMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* InactiveMeshComponent;

	FTimerHandle MeshTimeHandle;

	void ResetMesh();

public:
	// Sets default values for this actor's properties
	AOrb();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void Teleported();

	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UStaticMeshComponent* GetProjectileActiveMesh() const { return ActiveMeshComponent; }
	FORCEINLINE class UStaticMeshComponent* GetProjectileInactiveMesh() const { return InactiveMeshComponent; }


};
