// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Activatable.h"
#include "Portal.generated.h"

UCLASS()
class TELESHOOT_API APortal : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CompleteMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SpeedMesh;
	UMaterialInterface* CompleteMaterial, * SpeedMaterial;
public:
	// Sets default values for this actor's properties
	APortal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		FName LevelName;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void ChangeLevel();

	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }

	UPROPERTY(EditAnywhere, Category = Activates)
		TArray<AActivatable*> Activatable;

};
