// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleShoot.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine.h"
#include "Orb.h"
#include "Crystal.h"
#include "TeleShootCharacter.h"

// Sets default values
AProjectile::AProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->SetEnableGravity(false);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->InitBoxExtent(FVector(27.87f, 8.8f, 8.8f));

	ConstructorHelpers::FObjectFinder<UParticleSystem> ArbitraryParticleName(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Sparks.P_Sparks'"));
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ArbitraryParticleName"));

	if (ArbitraryParticleName.Succeeded()) {
		ParticleSystem->Template = ArbitraryParticleName.Object;
	}
	ParticleSystem->bAutoActivate = false;
	ParticleSystem->SetHiddenInGame(false);


	ShotSpeedOriginal = 800.f;
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = ShotSpeedOriginal;
	ProjectileMovement->MaxSpeed = ShotSpeedOriginal;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0;


	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Burnished_Steel.M_Metal_Burnished_Steel'"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	if (MeshConstruct.Succeeded())
		MeshComponent->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		MeshComponent->SetMaterial(0, MeshMaterial.Object);
	MeshComponent->bCastDynamicShadow = true;
	MeshComponent->CastShadow = true;
	MeshComponent->BodyInstance.SetCollisionProfileName("Projectile");
	MeshComponent->SetHiddenInGame(false);
	MeshComponent->CanCharacterStepUpOn = ECB_Yes;
	MeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	RootComponent = CollisionComp;
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InitialLifeSpan = 0;
}

void AProjectile::OnHit(class UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor->IsA(ACrystal::StaticClass())) {
		UWorld* const World = GetWorld();
		if (World != NULL) {
			ACharacter* Player = UGameplayStatics::GetPlayerCharacter(World, 0);
			if (Player != NULL) {
				FVector PlayerLocation = Player->GetActorLocation();
				ATeleShootCharacter* TeleshootPlayer = Cast<ATeleShootCharacter>(Player);
				if (TeleshootPlayer) {
					TeleshootPlayer->SwitchLocation(OtherActor->GetActorLocation(), false);
					OtherActor->SetActorLocation(PlayerLocation);
				}
			}
		}
		Destroy();
	}
	else if (OtherActor->IsA(AOrb::StaticClass())) {
		UWorld* const World = GetWorld();
		if (World != NULL) {
			ACharacter* Player = UGameplayStatics::GetPlayerCharacter(World, 0);
			if (Player != NULL) {
				FVector PlayerLocation = Player->GetActorLocation();
				ATeleShootCharacter* TeleshootPlayer = Cast<ATeleShootCharacter>(Player);
				if (TeleshootPlayer) {
					TeleshootPlayer->SwitchLocation(OtherActor->GetActorLocation(), true);
					AOrb* Orb = Cast<AOrb>(OtherActor);
					Orb->Teleported();
				}
			}
		}
		Destroy();
	}
	else if (OtherActor->IsA(ATeleShootCharacter::StaticClass())) {
		//Check if its bouncy one, then maybe have it switch with the original position it was shot at
		Destroy();
	}
	else//More for when there are different versions
	{
		ParticleSystem->ActivateSystem();
		UActorComponent* Mesh = FindComponentByClass<UMeshComponent>();
		if (Mesh != NULL)
			Mesh->DestroyComponent();
		ProjectileMovement->StopMovementImmediately();
		GetWorld()->GetTimerManager().SetTimer(SparksTimerHandle, this, &AProjectile::OnHitWall, .3, false);
	}
}

void AProjectile::OnHitWall() {
	GetWorld()->GetTimerManager().ClearTimer(SparksTimerHandle);
	Destroy();
}

void AProjectile::SetSpeed(float ShotSpeed) {
	ProjectileMovement->MaxSpeed = ShotSpeed;
	ProjectileMovement->SetVelocityInLocalSpace((ProjectileMovement->Velocity.ForwardVector)*(ShotSpeed));
}