// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Portal.h"
#include "TeleShootGameInstance.h"


// Sets default values
APortal::APortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Copper.M_Metal_Copper'"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	CompleteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompleteMesh"));
	SpeedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpeedMesh"));
	if (MeshConstruct.Succeeded())
		MeshComponent->SetStaticMesh(MeshConstruct.Object);
		CompleteMesh->SetStaticMesh(MeshConstruct.Object);
		SpeedMesh->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		MeshComponent->SetMaterial(0, MeshMaterial.Object);
	MeshComponent->bCastDynamicShadow = true;
	MeshComponent->CastShadow = true;
	MeshComponent->BodyInstance.SetCollisionProfileName("Interact");
	MeshComponent->SetHiddenInGame(false);
	MeshComponent->CanCharacterStepUpOn = ECB_No;
	MeshComponent->SetWorldScale3D(FVector(1, 0.02, 1.75));

	static ConstructorHelpers::FObjectFinder<UMaterial> StarMeshMaterial(TEXT("Material'/Game/SideScrollerCPP/Materials/GoalStarMaterial.GoalStarMaterial'"));
	if (StarMeshMaterial.Succeeded())
		CompleteMesh->SetMaterial(0, StarMeshMaterial.Object);
		SpeedMesh->SetMaterial(0, StarMeshMaterial.Object);
	CompleteMesh->bCastDynamicShadow = true;
	CompleteMesh->CastShadow = true;
	CompleteMesh->BodyInstance.SetCollisionProfileName("NoCollision");
	CompleteMesh->SetHiddenInGame(false);
	CompleteMesh->CanCharacterStepUpOn = ECB_No;
	CompleteMesh->SetWorldScale3D(FVector(.3, 11.9, .17));
	CompleteMesh->SetRelativeLocation(FVector(-30,0,66));

	SpeedMesh->bCastDynamicShadow = true;
	SpeedMesh->CastShadow = true;
	SpeedMesh->BodyInstance.SetCollisionProfileName("NoCollision");
	SpeedMesh->SetHiddenInGame(false);
	SpeedMesh->CanCharacterStepUpOn = ECB_No;
	SpeedMesh->SetWorldScale3D(FVector(.3, 11.9, .17));
	SpeedMesh->SetRelativeLocation(FVector(30, 0, 66));

	RootComponent = MeshComponent;
	CompleteMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpeedMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	CompleteMaterial = UMaterialInstanceDynamic::Create(CompleteMesh->GetMaterial(0), this);
	SpeedMaterial = UMaterialInstanceDynamic::Create(SpeedMesh->GetMaterial(0), this);

	UTeleShootGameInstance* GameInstance = Cast<UTeleShootGameInstance>(GetGameInstance());
	if (GameInstance->IsLevelComplete(LevelName) || true) {
		CompleteMaterial->SetScalarParameterValue("Desaturation", 0);
		CompleteMesh->SetMaterial(0, CompleteMaterial);
	}
	if (GameInstance->IsLevelSpeed(LevelName)) {
		SpeedMaterial->SetScalarParameterValue("Desaturation", 0);
		SpeedMesh->SetMaterial(0, SpeedMaterial);
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::ChangeLevel() {
	UTeleShootGameInstance* GameInstance = Cast<UTeleShootGameInstance>(GetGameInstance());
	GameInstance->ChangeLevel(LevelName);
}
