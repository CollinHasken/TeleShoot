// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GunUpgrade.generated.h"

UENUM(BlueprintType)		
enum class UpgradeType : uint8
{
	TeleportTo 	UMETA(DisplayName = "TeleportTo"),
	BringTo 	UMETA(DisplayName = "BringTo"),
	Switch	UMETA(DisplayName = "Switch"),
	ThreeD 	UMETA(DisplayName = "ThreeD"),
	TimeSlow 	UMETA(DisplayName = "TimeSlow"),
	Charge 	UMETA(DisplayName = "Charge")
};

UCLASS()
class TELESHOOT_API AGunUpgrade : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

public:	
	// Sets default values for this actor's properties
	AGunUpgrade();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UFUNCTION()
		void OnActorBeginOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level)
		UpgradeType Upgrade;
};
