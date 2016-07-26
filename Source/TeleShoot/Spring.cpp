// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Spring.h"
#include "TeleShootCharacter.h"

// Sets default values
ASpring::ASpring()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Comp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Trigger");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASpring::OnActorBeginOverlap);
	CollisionComp->SetEnableGravity(false);
	CollisionComp->CanCharacterStepUpOn = ECB_Yes;
	CollisionComp->InitBoxExtent(FVector(48, 48, 14.5));
	CollisionComp->SetRelativeLocation(FVector(0, 0, 46));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Ground_Gravel.M_Ground_Gravel'"));
	//Create
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	//Set Mesh
	if (MeshConstruct.Succeeded())
		MeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		MeshComponent->SetMaterial(0, MeshMaterial.Object);
	MeshComponent->SetHiddenInGame(false);
	MeshComponent->SetRelativeLocation(FVector(0, 0, 32));
	MeshComponent->SetRelativeScale3D(FVector(1, 1, .178));
	MeshComponent->BodyInstance.SetCollisionProfileName("BlockAll");

	RootComponent = MeshComponent;
	CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ASpring::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpring::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpring::OnActorBeginOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(ATeleShootCharacter::StaticClass())) {
		ATeleShootCharacter* Player = Cast<ATeleShootCharacter>(OtherActor);
		Player->LaunchCharacter(FVector(0, 0, SpringForce), false, true);
	}
}