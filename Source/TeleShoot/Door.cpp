// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Door.h"


ADoor::ADoor() {
	PrimaryActorTick.bCanEverTick = true;

	UBoxComponent* Dumby = CreateDefaultSubobject<UBoxComponent>(TEXT("Dummby"));
	Dumby->BodyInstance.SetCollisionProfileName("IgnoreAll");
	Dumby->SetEnableGravity(false);
	Dumby->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	Dumby->CanCharacterStepUpOn = ECB_No;
	Dumby->InitBoxExtent(FVector(20, 110, 140));

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Comp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("BlockAll");
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->InitBoxExtent(FVector(6, 49, 106));
	CollisionComp->SetRelativeLocation(FVector(0, -50, 0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Door.SM_Door'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Props/Materials/M_Door.M_Door'"));
	//Create
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	//Set Mesh
	if (MeshConstruct.Succeeded())
		MeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		MeshComponent->SetMaterial(0, MeshMaterial.Object);
	MeshComponent->SetHiddenInGame(false);
	MeshComponent->SetRelativeLocation(FVector(0, 45, -102));

	//Deferred Attachment (Ty Nick W.! Actual attach gets done after blueprint stuff)
	RootComponent = Dumby;
	CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeshComponent->AttachToComponent(CollisionComp, FAttachmentTransformRules::KeepRelativeTransform);
	IsOpenning = false;
	IsClosing = false;
	EndRotation = FRotator(0, 0, 0);
}


void ADoor::BeginPlay()
{
	BeginRotation = CollisionComp->GetComponentRotation();
}

void ADoor::Activate()
{
	IsOpenning = true;
	IsClosing = false;
}

void ADoor::Deactivate()
{
	IsClosing = true;
	IsOpenning = false;
}

void ADoor::Tick(float DeltaSeconds)
{
	if (IsClosing) {
		if (CollisionComp->GetComponentRotation() != BeginRotation) {
			CollisionComp->SetWorldRotation(FMath::RInterpTo(CollisionComp->GetComponentRotation(), BeginRotation, DeltaSeconds, .8f));
		}
		else
			IsClosing = false;
	}
	else if (IsOpenning) {
		if (CollisionComp->GetComponentRotation() != EndRotation) {
			CollisionComp->SetWorldRotation(FMath::RInterpTo(CollisionComp->GetComponentRotation(), EndRotation, DeltaSeconds, .8f));
		}
		else
			IsClosing = false;
	}
}



