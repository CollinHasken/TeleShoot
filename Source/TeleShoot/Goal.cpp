// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Goal.h"
#include "TeleShootCharacter.h"
#include "TeleShootGameInstance.h"
#include "Engine.h"


// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Burnished_Steel.M_Metal_Burnished_Steel'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (MeshConstruct.Succeeded())
		Mesh->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		Mesh->SetMaterial(0, MeshMaterial.Object);
	Mesh->bCastDynamicShadow = true;
	Mesh->CastShadow = true;
	Mesh->BodyInstance.SetCollisionProfileName("NoCollision");
	Mesh->SetHiddenInGame(false);
	Mesh->CanCharacterStepUpOn = ECB_Yes;
	Mesh->SetRelativeScale3D(FVector(0.237305, 0.237305, 0.237305));
	Mesh->SetRelativeLocation(FVector(0, 0, 0));

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("OverlapOnlyPawn");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnActorBeginOverlap);
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->InitSphereRadius(400);
	CollisionComp->SetRelativeLocation(FVector(0, 0, 42));

	RootComponent = Mesh;
	CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	StartTime = GetWorld()->GetRealTimeSeconds();
	GEngine->AddOnScreenDebugMessage(1, 10000.f, FColor::Red, FString::Printf(TEXT("Par: %.2f"), ParTime));
}

// Called every frame
void AGoal::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if(!BeatSpeed)
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("Time: %.2f"), GetWorld()->GetTimeSeconds() - StartTime));
}

void AGoal::OnActorBeginOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(ATeleShootCharacter::StaticClass())) {
		ATeleShootCharacter* Character = Cast<ATeleShootCharacter>(OtherActor);
		Character->EndLevel();
		GetWorld()->GetTimerManager().SetTimer(EndLevelTimer, this, &AGoal::LevelEnded, 1, false);
		if ((GetWorld()->GetTimeSeconds() - StartTime) <= ParTime)
			BeatSpeed = true;
	}
}

void AGoal::LevelEnded() {
	UTeleShootGameInstance* GameInstance = Cast<UTeleShootGameInstance>(GetGameInstance());
	GameInstance->EndLevel(LevelName, BeatSpeed);
}