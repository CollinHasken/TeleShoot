// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Orb.h"


// Sets default values
AOrb::AOrb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_Yes;
	CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
	CollisionComp->SetCollisionProfileName("NoCollision");
	CollisionComp->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	RootComponent = CollisionComp;


	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
	//Create
	ActiveMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActiveMeshComponent"));

	//Set Mesh
	if (MeshConstruct.Succeeded())
		ActiveMeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		ActiveMeshComponent->SetMaterial(0, MeshMaterial.Object);
	ActiveMeshComponent->SetHiddenInGame(false);
	ActiveMeshComponent->SetCollisionProfileName("Orb");

	//Deferred Attachment (Ty Nick W.! Actual attach gets done after blueprint stuff)
	ActiveMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UMaterial> Mesh2Material(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile'"));
	//Create
	InactiveMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InactiveMeshComponent"));

	//Set Mesh
	if (MeshConstruct.Succeeded())
		InactiveMeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		InactiveMeshComponent->SetMaterial(0, Mesh2Material.Object);
	InactiveMeshComponent->SetHiddenInGame(true);
	InactiveMeshComponent->SetCollisionProfileName("NoCollision");

	//Deferred Attachment (Ty Nick W.! Actual attach gets done after blueprint stuff)
	InactiveMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void AOrb::Teleported() {
	ActiveMeshComponent->SetHiddenInGame(true);
	ActiveMeshComponent->SetCollisionProfileName("NoCollision");
	InactiveMeshComponent->SetHiddenInGame(false);
	GetWorld()->GetTimerManager().SetTimer(MeshTimeHandle, this, &AOrb::ResetMesh, 2);
}

void AOrb::ResetMesh() {
	ActiveMeshComponent->SetHiddenInGame(false);
	ActiveMeshComponent->SetCollisionProfileName("Orb");
	InactiveMeshComponent->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AOrb::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
	CollisionComp->SetCollisionProfileName("NoCollision");

}

// Called every frame
void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

