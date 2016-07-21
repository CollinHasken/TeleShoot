// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Lever.h"
#include "Activatable.h"

// Sets default values
ALever::ALever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Water_Ocean.M_Water_Ocean'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (MeshConstruct.Succeeded())
		Mesh->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		Mesh->SetMaterial(0, MeshMaterial.Object);
	Mesh->bCastDynamicShadow = true;
	Mesh->CastShadow = true;
	Mesh->BodyInstance.SetCollisionProfileName("Interact");
	Mesh->SetHiddenInGame(false);
	Mesh->CanCharacterStepUpOn = ECB_Yes;
	Mesh->SetRelativeScale3D(FVector(0.237305, 0.237305, 0.237305));
	Mesh->SetRelativeLocation(FVector(0, 0, 0));

	RootComponent = Mesh;
	Activated = false;
}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALever::Switch() {
	if (Activatable.Num() != 0) {
		if (!Activated)
			for(AActivatable* Activate : Activatable)
				Activate->Activate();
		else
			for (AActivatable* Activate : Activatable)
				Activate->Deactivate();
		Activated = !Activated;
	}
}