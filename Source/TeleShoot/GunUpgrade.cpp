// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "GunUpgrade.h"
#include "TeleshootCharacter.h"
#include "TeleShootGameInstance.h"


// Sets default values
AGunUpgrade::AGunUpgrade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Burnished_Steel.M_Metal_Burnished_Steel'"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (MeshConstruct.Succeeded())
		MeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		MeshComponent->SetMaterial(0, MeshMaterial.Object);
	MeshComponent->bCastDynamicShadow = true;
	MeshComponent->CastShadow = true;
	MeshComponent->BodyInstance.SetCollisionProfileName("NoCollision");
	MeshComponent->SetHiddenInGame(false);
	MeshComponent->CanCharacterStepUpOn = ECB_Yes;
	MeshComponent->SetRelativeScale3D(FVector(0.237305, 0.237305, 0.237305));
	MeshComponent->SetRelativeLocation(FVector(0, 0, 0));

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("OverlapOnlyPawn");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGunUpgrade::OnActorBeginOverlap);
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->InitSphereRadius(400);
	CollisionComp->SetRelativeLocation(FVector(0, 0, 42));

	RootComponent = MeshComponent;
	CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGunUpgrade::BeginPlay()
{
	Super::BeginPlay();
	ATeleShootCharacter* Character = Cast<ATeleShootCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Character && Character->HasGunUpgrade(Upgrade)) {
		Destroy();
	}
}

// Called every frame
void AGunUpgrade::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGunUpgrade::OnActorBeginOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(ATeleShootCharacter::StaticClass())) {
		ATeleShootCharacter* Character = Cast<ATeleShootCharacter>(OtherActor);
		Character->UpgradeGun(Upgrade);
		UTeleShootGameInstance* Game = Cast<UTeleShootGameInstance>(GetGameInstance());
		Game->UpgradeGun(Upgrade);
		Destroy();
	}
}