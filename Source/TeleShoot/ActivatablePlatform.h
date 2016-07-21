// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Activatable.h"
#include "ActivatablePlatform.generated.h"

/**
*
*/
UCLASS()
class TELESHOOT_API AActivatablePlatform : public AActivatable
{
	GENERATED_BODY()
		AActivatablePlatform();

	virtual void Activate() override;
	virtual void Deactivate() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Opens)
		FVector EndLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Opens)
		float MoveSpeed;

private:
	FVector BeginLocation;
	bool Activated;
	bool Deactivated;
};
