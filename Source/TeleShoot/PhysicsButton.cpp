// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "PhysicsButton.h"
#include "TeleShootCharacter.h"
#include "HoldableBox.h"
#include "Activatable.h"

// Sets default values
APhysicsButton::APhysicsButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Comp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Trigger");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APhysicsButton::OnActorBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &APhysicsButton::OnActorEndOverlap);
	CollisionComp->SetEnableGravity(false);
	CollisionComp->CanCharacterStepUpOn = ECB_Yes;
	CollisionComp->InitBoxExtent(FVector(40, 40, 14.5));
	CollisionComp->SetRelativeLocation(FVector(0, 0, 220));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseConstruct(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles'"));
	//Create
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));

	//Set Mesh
	if (BaseConstruct.Succeeded())
		BaseMesh->SetStaticMesh(BaseConstruct.Object);
	if (BaseConstruct.Succeeded())
		BaseMesh->SetMaterial(0, BaseMaterial.Object);
	BaseMesh->SetHiddenInGame(false);
	BaseMesh->SetRelativeLocation(FVector(0, 0, 195));
	BaseMesh->SetRelativeScale3D(FVector(0.83, 0.83, .1));
	BaseMesh->BodyInstance.SetCollisionProfileName("BlockAll");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ButtonConstruct(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> ButtonMaterial(TEXT("Material'/Game/SideScrollerCPP/Materials/ButtonMaterial.ButtonMaterial'"));
	//Create
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));

	//Set Mesh
	if (ButtonConstruct.Succeeded())
		ButtonMesh->SetStaticMesh(ButtonConstruct.Object);
	if (ButtonConstruct.Succeeded())
		ButtonMesh->SetMaterial(0, ButtonMaterial.Object);
	ButtonMesh->SetHiddenInGame(false);
	ButtonMesh->SetRelativeLocation(FVector(0, 0, 80));
	ButtonMesh->SetRelativeScale3D(FVector(0.8, 0.8, 2));
	ButtonMesh->BodyInstance.SetCollisionProfileName("BlockAll");

	RootComponent = BaseMesh;
	ButtonMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ActorInside = BoxInside = false;
}

// Called when the game starts or when spawned
void APhysicsButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicsButton::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if ((ActorInside || BoxInside)){
		if (ButtonMesh->RelativeLocation.Z > -120) {
			ButtonMesh->AddRelativeLocation(FVector(0, 0, -3));
		}
	}
	else if (ButtonMesh->RelativeLocation.Z < 80) {
		ButtonMesh->AddRelativeLocation(FVector(0, 0, 3));
	}
}

void APhysicsButton::OnActorBeginOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(ATeleShootCharacter::StaticClass())) {
		ActorInside = true;
	}
	else if (OtherActor->IsA(AHoldableBox::StaticClass())) {
		BoxInside = true;
	}
	if ((ActorInside && !BoxInside) || (!ActorInside && BoxInside)) {
		for (AActivatable* Activate : Activatable)
			Activate->Activate();
	}
}

void APhysicsButton::OnActorEndOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(ATeleShootCharacter::StaticClass())) {
		ActorInside = false;
	}
	else if (OtherActor->IsA(AHoldableBox::StaticClass())) {
		BoxInside = false;
	}
	if (!ActorInside && !BoxInside) {
		for (AActivatable* Activate : Activatable)
			Activate->Deactivate();
	}
}