// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Slot.h"
#include "Activatable.h"

// Sets default values
ASlot::ASlot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Slot");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASlot::OnActorBeginOverlap);
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->InitSphereRadius(100);

	CenterCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CenterComp"));
	CenterCollisionComp->BodyInstance.SetCollisionProfileName("Slot");
	CenterCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASlot::OnActorBeginOverlapCenter);
	CenterCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ASlot::OnActorEndOverlapCenter);
	CenterCollisionComp->SetEnableGravity(false);
	CenterCollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CenterCollisionComp->CanCharacterStepUpOn = ECB_No;
	CenterCollisionComp->InitSphereRadius(16);

	ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));

	if (Particle.Succeeded()) {
		ParticleSystem->Template = Particle.Object;
	}
	ParticleSystem->bAutoActivate = false;
	ParticleSystem->SetHiddenInGame(false);
	ParticleSystem->SetRelativeLocation(FVector(0, 0, 0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Water_Ocean.M_Water_Ocean'"));
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




	RootComponent = CollisionComp;
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CenterCollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
void ASlot::OnActorBeginOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(ACrystal::StaticClass())) {
		ACrystal* Crystal = Cast<ACrystal>(OtherActor);
		Crystal->MoveActorTo(GetActorLocation());
	}
}

void ASlot::OnActorBeginOverlapCenter(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	RootComponent = CollisionComp;
	if (OtherActor->IsA(ACrystal::StaticClass())) {
		ParticleSystem->ActivateSystem();
		if (Activatable != NULL) {
			Activatable->Activate();
		}
	}
}

void ASlot::OnActorEndOverlapCenter(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	RootComponent = CollisionComp;
	if (OtherActor->IsA(ACrystal::StaticClass())) {
		ParticleSystem->DeactivateSystem();
		if (Activatable != NULL) {
			Activatable->Deactivate();
		}
	}
}

// Called when the game starts or when spawned
void ASlot::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}