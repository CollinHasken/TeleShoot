// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Activatable.h"
#include "Door.generated.h"

/**
*
*/
UCLASS()
class TELESHOOT_API ADoor : public AActivatable
{
	GENERATED_BODY()

		ADoor();

	virtual void Activate() override;
	virtual void Deactivate() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UBoxComponent* CollisionComp;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Opens)
		FRotator EndRotation;

private:
	bool IsOpenning;
	bool IsClosing;
	FRotator BeginRotation;
};
