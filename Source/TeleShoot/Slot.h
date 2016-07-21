// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Crystal.h"
#include "Slot.generated.h"

UCLASS()
class TELESHOOT_API ASlot : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CenterCollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;


public:
	// Sets default values for this actor's properties
	ASlot();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return Mesh; }

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
		void OnActorBeginOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnActorBeginOverlapCenter(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnActorEndOverlapCenter(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = Activates)
	class AActivatable* Activatable;

};
