// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "HoldableBox.h"


// Sets default values
AHoldableBox::AHoldableBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles'"));
	//Create
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	//Set Mesh
	if (MeshConstruct.Succeeded())
		MeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		MeshComponent->SetMaterial(0, MeshMaterial.Object);
	MeshComponent->SetHiddenInGame(false);
	MeshComponent->SetRelativeScale3D(FVector(.38, .38, .38));
	MeshComponent->BodyInstance.SetCollisionProfileName("Interact");
	MeshComponent->SetSimulatePhysics(true);

	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AHoldableBox::BeginPlay()
{
	Super::BeginPlay();
	CurrentY = GetActorLocation().Y;
}

// Called every frame
void AHoldableBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector BoxLocation(GetActorLocation());
	if (BoxLocation.Y != CurrentY) {
		BoxLocation.Y = CurrentY;
		SetActorLocation(BoxLocation);
	}
}

void AHoldableBox::PickUp() {
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AHoldableBox::Drop(FVector Location, FRotator Rotation) {
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorLocationAndRotation(Location, Rotation);
	MeshComponent->ComponentVelocity = FVector(0, 0, 0);
}

