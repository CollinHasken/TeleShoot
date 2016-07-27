// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "ActivatablePlatform.h"

AActivatablePlatform::AActivatablePlatform() {
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Brushed_Nickel.M_Metal_Brushed_Nickel'"));
	//Create
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	//Set Mesh
	if (MeshConstruct.Succeeded())
		MeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		MeshComponent->SetMaterial(0, MeshMaterial.Object);
	MeshComponent->SetHiddenInGame(false);
	MeshComponent->SetRelativeScale3D(FVector(2.21, 1.37, .178));

	RootComponent = MeshComponent;

	Activated = Deactivated = false;
	MoveSpeed = 250;

}

void AActivatablePlatform::BeginPlay()
{
	Super::BeginPlay();
	BeginLocation = GetActorLocation();
	SetActorTickEnabled(false);
}

void AActivatablePlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Activated || Deactivated) {
		if (Activated && (GetActorLocation() - EndLocation).Size() < 5) {
			SetActorLocation(EndLocation);
			Activated = false;
			SetActorTickEnabled(false);
		}
		else if (Deactivated && (GetActorLocation() - BeginLocation).Size() < 5) {
			SetActorLocation(BeginLocation);
			Deactivated = false;
			SetActorTickEnabled(false);
		}
		else {
			FVector NewLocation;
			if (Activated)
				NewLocation = FMath::VInterpConstantTo(GetActorLocation(), EndLocation, DeltaSeconds, MoveSpeed);
			else
				NewLocation = FMath::VInterpConstantTo(GetActorLocation(), BeginLocation, DeltaSeconds, MoveSpeed);
			SetActorLocation(NewLocation);
		}
	}
}

void AActivatablePlatform::Activate()
{
	Activated = true;
	Deactivated = false;
	SetActorTickEnabled(true);
}

void AActivatablePlatform::Deactivate()
{
	Deactivated = true;
	Activated = false;
	SetActorTickEnabled(true);
}




