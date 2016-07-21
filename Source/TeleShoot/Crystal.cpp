// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Crystal.h"


// Sets default values
ACrystal::ACrystal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CollisionComp->InitCapsuleSize(25, 51);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_Yes;

	RootComponent = CollisionComp;


	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile'"));
	//Create
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	//Set Mesh
	if (MeshConstruct.Succeeded())
		MeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		MeshComponent->SetMaterial(0, MeshMaterial.Object);
	MeshComponent->SetHiddenInGame(false);
	MeshComponent->SetRelativeLocation(FVector(0, 0, -50));

	//Deferred Attachment (Ty Nick W.! Actual attach gets done after blueprint stuff)
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HasNewLocation = false;
}

// Called when the game starts or when spawned
void ACrystal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasNewLocation) {
		FVector ActorLocation = GetRootComponent()->GetComponentLocation();
		if ((ActorLocation - NewLocation).Size() < 0.5) {
			RootComponent->SetWorldLocation(NewLocation);
			HasNewLocation = false;
		}
		else if ((ActorLocation - NewLocation).Size() < 5) {
			RootComponent->SetWorldLocation(FMath::Lerp(ActorLocation, NewLocation, .9f));
		}
		else {
			RootComponent->SetWorldLocation(FMath::Lerp(ActorLocation, NewLocation, .05f));
		}
	}
}

void ACrystal::MoveActorTo(FVector NewLocation) {
	HasNewLocation = true;
	this->NewLocation = NewLocation;
}

