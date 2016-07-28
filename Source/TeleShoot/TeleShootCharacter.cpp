// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TeleShoot.h"
#include "TeleShootCharacter.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "Projectile.h"
#include "Portal.h"
#include "Lever.h"
#include "HoldableBox.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "GunUpgrade.h"
#include "TeleShootGameInstance.h"
#include "GameFramework/PlayerStart.h"

ATeleShootCharacter::ATeleShootCharacter()
{
	SprintSpeed = 800;
	WalkSpeed = 450;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	BoxSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("BoxSpring"));
	BoxSpring->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxSpring->bInheritPitch = BoxSpring->bInheritRoll = false;
	BoxSpring->bInheritYaw = true;
	BoxSpring->bDoCollisionTest = true;
	BoxSpring->TargetArmLength = 160;
	
	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshConstruct(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube_Chamfer.1M_Cube_Chamfer'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles'"));
	//Create
	HoldBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));

	//Set Mesh
	if (MeshConstruct.Succeeded())
		HoldBox->SetStaticMesh(MeshConstruct.Object);
	if (MeshMaterial.Succeeded())
		HoldBox->SetMaterial(0, MeshMaterial.Object);
	HoldBox->SetRelativeScale3D(FVector(.7, .7, .7));
	HoldBox->BodyInstance.SetCollisionProfileName("NoCollision");
	HoldBox->SetComponentTickEnabled(false);
	HoldBox->SetHiddenInGame(true);
	HoldBox->AttachToComponent(BoxSpring, FAttachmentTransformRules::KeepRelativeTransform);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->InitBoxExtent(FVector(115, 150, 105));
	InteractBox->BodyInstance.SetCollisionProfileName("Interact");
	InteractBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 900.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;


	UWorld* const World = GetWorld();
	if (World != NULL)
		for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
			APlayerStart* PlayerStart = *ActorItr;
			CurrentYAxis = PlayerStart->GetActorLocation().Y;
		}
	HoldingBox = NULL;
	OriginalBoomLength = CameraBoom->TargetArmLength;
	LPad.Set(200, 0, 0);
	RPad.Set(-200, 0, 0);
	TPad.Set(0, 0, -200);
	BPad.Set(0, 0, 200);
	GunOffset = 100;
	FireRate = .3;
	OriginalFireSpeed = 1500;
	FireSpeed = OriginalFireSpeed;
	FireSpeedMax = 5000;
	FireSpeedRate = 1400;
	CameraMax = 500;
	CameraMoveMax = 600;
	CameraMoveRate = 500;
	CameraRate = .9;
	CameraReturnRate = .4;
	ShotTime = -FireRate;
	DoubleJumpHeight = 800;
	MouseClicked = JustTeleported = AtPeak = IsSliding = TryingToUncrouch = false;
	CanMove = true;
	// Note: The skeletal mesh and animation blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATeleShootCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up game play key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("DoubleJump", IE_Pressed, this, &ATeleShootCharacter::TeleJump);
	InputComponent->BindAction("Duck", IE_Pressed, this, &ATeleShootCharacter::Duck);
	InputComponent->BindAction("Duck", IE_Released, this, &ATeleShootCharacter::Unduck);
	InputComponent->BindAxis("MoveRight", this, &ATeleShootCharacter::MoveRight);
	InputComponent->BindAction("SprintToggle", IE_Pressed, this, &ATeleShootCharacter::WalkOn);
	InputComponent->BindAction("SprintToggle", IE_Released, this, &ATeleShootCharacter::WalkOff);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ATeleShootCharacter::Interact);

	InputComponent->BindTouch(IE_Pressed, this, &ATeleShootCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ATeleShootCharacter::TouchStopped);


	InputComponent->BindAction(TEXT("LeftClick"), IE_Pressed, this, &ATeleShootCharacter::Fire2DPressed);
	InputComponent->BindAction(TEXT("LeftClick"), IE_Released, this, &ATeleShootCharacter::Fire2DRelease);
	InputComponent->BindAction(TEXT("RightClick"), IE_Pressed, this, &ATeleShootCharacter::Fire3DPressed);
	InputComponent->BindAction(TEXT("RightClick"), IE_Released, this, &ATeleShootCharacter::Fire3DRelease);
}

void ATeleShootCharacter::BeginPlay() {
	Super::BeginPlay();
	OriginalCameraPosition = SideViewCameraComponent->GetComponentLocation() - GetActorLocation();
	OriginalYAxis = GetActorLocation().Y;
	UTeleShootGameInstance* Game = Cast<UTeleShootGameInstance>(GetGameInstance());
	Game->UpdateUpgrades(GunUpgrades);
}

void ATeleShootCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	FString size = FString::SanitizeFloat(GetVelocity().Size());
	FVector ActorLocation = GetActorLocation();
	FVector CameraLocation = SideViewCameraComponent->GetComponentLocation();
	if (JustTeleported && GetCharacterMovement()->Velocity.Z == 0) {
		if (AtPeak) {
			JustTeleported = false;
			AtPeak = false;
		}
		AtPeak = true;
	}
	else
		AtPeak = false;

	if (ActorLocation.Y != CurrentYAxis) {
		ActorLocation.Y = CurrentYAxis;
		SetActorLocation(ActorLocation);
	}
	if (MouseClicked) {
		//DrawDebugLine(GetWorld(), GetActorLocation(), SideViewCameraComponent->GetComponentLocation(), FColor(0, 255, 0), false, 40, 0, 1);
		//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + OriginalCameraPosition, FColor(255, 0, 0), false, 40, 0, 4);
		if (FireSpeed < FireSpeedMax) {
			FireSpeed = FMath::Clamp(FireSpeed + DeltaTime * FireSpeedRate, 0.f, FireSpeedMax);
		}
	}
	CheckMousePosition(ActorLocation, CameraLocation, DeltaTime);
	if (HoldingBox != NULL)
		UpdateBoxPosition();
}

void ATeleShootCharacter::UpdateBoxPosition() {
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FVector MousePosition, MouseDirection;
	if (PlayerController) {
		if (PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection)) {
			if (ProjectileClass != NULL) {
				FVector EndLocation = MousePosition + CameraBoom->TargetArmLength * MouseDirection;
				FVector loc(GetActorLocation());
				FVector projectile(loc.X - EndLocation.X, 0, loc.Z - EndLocation.Z);
				FRotator SpawnRotation = projectile.Rotation();
				SpawnRotation.Normalize();

				BoxSpring->SetWorldRotation(SpawnRotation);
			}
		}
	}
}


void ATeleShootCharacter::MoveRight(float Value)
{
	// add movement in that direction
	if (!IsSliding && CanMove)
		AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
}

void ATeleShootCharacter::TeleJump() {
	if (JustTeleported) {
		GetCharacterMovement()->Launch((FVector(0, 0, DoubleJumpHeight)));
		JustTeleported = false;
	}
}

void ATeleShootCharacter::Duck() {
	if (!GetCharacterMovement()->IsFalling() && !IsSliding && CanMove) {
		if (!GetCharacterMovement()->Velocity.IsNearlyZero()) {
			IsSliding = true;
			GetCharacterMovement()->MaxWalkSpeedCrouched = 3000;
			if(GetActorForwardVector().X >= 0)
				LaunchCharacter(FVector(2000, 0, 0), true, true);
			else
				LaunchCharacter(FVector(-2000, 0, 0), true, true);
			GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &ATeleShootCharacter::DoneSlide, 0.75, false);
		}
		Crouch();
	}
}

void ATeleShootCharacter::DoneSlide() {
	if (TryingToUncrouch) {
		UnCrouch();
		TryingToUncrouch = false;
	}
	IsSliding = false;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300;

}

void ATeleShootCharacter::Unduck() {
	if (!IsSliding) {
		GetCharacterMovement()->MaxWalkSpeedCrouched = 300;
		UnCrouch();
		TryingToUncrouch = false;
	}
	else {
		TryingToUncrouch = true;
	}
}

void ATeleShootCharacter::WalkOn() {
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ATeleShootCharacter::WalkOff() {
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ATeleShootCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	if (CanMove)
		Jump();
}

void ATeleShootCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ATeleShootCharacter::EndLevel() {
	CanMove = false;
	//Play animation
}

void ATeleShootCharacter::SwitchLocation(FVector Location, bool CanJumpAgain) {
	if (!CanMove)
		return;
	FVector CameraPosition = SideViewCameraComponent->GetComponentLocation();
	SetActorLocation(Location);
	CurrentYAxis = Location.Y;
	SideViewCameraComponent->SetWorldLocation(CameraPosition);
	if (GetCharacterMovement()->Velocity.Z == 0)
		GetCharacterMovement()->Launch(FVector(0, 0, 100));
	JustTeleported = CanJumpAgain;
}

void ATeleShootCharacter::Interact() {
	if (!CanMove)
		return;
	if (HoldingBox != NULL) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		FVector MousePosition, MouseDirection;
		if (PlayerController) {
			if (PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection)) {
				if (ProjectileClass != NULL) {
					FVector EndLocation = MousePosition + CameraBoom->TargetArmLength * MouseDirection;
					FVector loc(GetActorLocation());
					FVector projectile(loc.X - EndLocation.X, 0, loc.Z - EndLocation.Z);
					FRotator Rotation = projectile.Rotation();
					Rotation.Normalize();
					projectile.Set(EndLocation.X - loc.X, 0, EndLocation.Z - loc.Z);
					projectile.Normalize();
					FVector Location = loc + projectile * 170;

					HoldingBox->Drop(Location, Rotation);
				}
			}
		}
		HoldBox->SetComponentTickEnabled(false);
		HoldBox->SetHiddenInGame(true);
		HoldingBox = NULL;
	}
	else {
		TArray<AActor*> OverlappingActors;
		InteractBox->GetOverlappingActors(OverlappingActors);
		for (int i = OverlappingActors.Num(); i > 0; i--) {
			if (OverlappingActors[i - 1]->IsA(APortal::StaticClass())) {
				APortal* Portal = Cast<APortal>(OverlappingActors[i - 1]);
				Portal->ChangeLevel();
			}
			else if (OverlappingActors[i - 1]->IsA(ALever::StaticClass())) {
				ALever* Lever = Cast<ALever>(OverlappingActors[i - 1]);
				Lever->Switch();
			}
			else if (OverlappingActors[i - 1]->IsA(AHoldableBox::StaticClass())) {
				HoldBox->SetComponentTickEnabled(true);
				HoldBox->SetHiddenInGame(false);
				HoldingBox = Cast<AHoldableBox>(OverlappingActors[i - 1]);
				HoldingBox->PickUp();
			}
		}
	}
}

void ATeleShootCharacter::UpgradeGun(UpgradeType Upgrade) {
	switch (Upgrade) {
		case UpgradeType::TeleportTo :
			GunUpgrades[0] = true;
			break;
		default:
			break;
	}
}

bool ATeleShootCharacter::HasGunUpgrade(UpgradeType Upgrade)
{
	switch (Upgrade) {
	case UpgradeType::TeleportTo:
		return GunUpgrades[0];
	default:
		return false;
	}
}

void ATeleShootCharacter::CheckMousePosition(FVector ActorLocation, FVector CameraLocation, float DeltaTime) {
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		FVector2D HalfWay = ViewportSize / 2;
		FVector2D MaxPosition(ViewportSize.X - HalfWay.X, ViewportSize.Y - HalfWay.Y);
		MaxPosition.Normalize();
		MaxPosition *= CameraMoveMax;
		FVector2D CurrentCameraOffset(CameraLocation.X - (ActorLocation.X + OriginalCameraPosition.X), (ActorLocation.Z + OriginalCameraPosition.Z) - CameraLocation.Z);
		float MouseX, MouseY;
		if (CurrentCameraOffset.Size() < MaxPosition.Size() && PlayerController->GetMousePosition(MouseX, MouseY)) {
			FVector2D MousePosition(MouseX - HalfWay.X, MouseY - HalfWay.Y);
			CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, FMath::Max(OriginalBoomLength, OriginalBoomLength + (MousePosition.Size() / HalfWay.Size()) * CameraMax), DeltaTime, CameraRate);
			FVector MouseNormalized(MousePosition.X, 0, -MousePosition.Y);
			MouseNormalized.Normalize();
			FVector NewLocation = ActorLocation + OriginalCameraPosition + (MousePosition.Size() / HalfWay.Size()) * MouseNormalized * CameraMoveMax;
			NewLocation.Y = CameraLocation.Y;
			SideViewCameraComponent->SetWorldLocation(FMath::Lerp(CameraLocation, NewLocation, .01f));
		}
		else {
			if (CameraBoom->TargetArmLength - OriginalBoomLength < 20) {
				CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, OriginalBoomLength, DeltaTime, .9f);
			}
			else {
				CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, OriginalBoomLength, DeltaTime, CameraReturnRate);
			}
			FVector CameraPosition = OriginalCameraPosition + GetActorLocation();
			CameraPosition.Y = CameraLocation.Y;
			if ((CameraPosition - CameraLocation).Size() < 10) {
				SideViewCameraComponent->SetWorldLocation(FMath::Lerp(CameraLocation, CameraPosition, .7f));
			}
			else {
				SideViewCameraComponent->SetWorldLocation(FMath::Lerp(CameraLocation, CameraPosition, .05f));
			}
		}
	}
}
void ATeleShootCharacter::Fire2DPressed() {
	MouseClicked = true;
}

void ATeleShootCharacter::Fire2DRelease() {
	MouseClicked = false;

	UWorld* const World = GetWorld();
	if (World == NULL || World->GetTimeSeconds() < ShotTime + FireRate || !CanMove || !GunUpgrades[0])
		return;
	FVector MousePosition;
	FVector MouseDirection;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		if (PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection)) {
			if (ProjectileClass != NULL) {
				FVector EndLocation = MousePosition + CameraBoom->TargetArmLength * MouseDirection;
				const FName TraceTag("MyTraceTag");
				World->DebugDrawTraceTag = TraceTag;

				FCollisionQueryParams TraceParams(FName(TEXT("CameraTrace")), true, this);
				TraceParams.bTraceAsyncScene = true;
				TraceParams.bReturnPhysicalMaterial = true;
				TraceParams.AddIgnoredActor(this);
				TraceParams.AddIgnoredActor(GetOwner());
				TraceParams.TraceTag = TraceTag;

				FVector loc(GetActorLocation());
				FVector projectile(EndLocation.X - loc.X, 0, EndLocation.Z - loc.Z);

				//DrawDebugLine(World, MousePosition, EndLocation, FColor(0, 255, 0), false, 40, 0, 1);
				//DrawDebugLine(World, GetActorLocation(), GetActorLocation() + projectile, FColor(0, 100, 100), false, 40, 0, 1);

				FRotator SpawnRotation = projectile.Rotation();
				SpawnRotation.Normalize();

				//FVector SpawnLocation = loc;
				//SpawnLocation += GunOffset * SpawnRotation.RotateVector(SpawnLocation + FVector(GunOffset,0,0));
				projectile.Normalize();
				FVector SpawnLocation = loc + projectile * GunOffset;

				//DrawDebugLine(World, GetActorLocation(), SpawnLocation, FColor(100, 100, 100), false, 40, 0, 2);

				AProjectile* bullet = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
				bullet->SetSpeed(FireSpeed);
				FireSpeed = OriginalFireSpeed;
				ShotTime = World->GetTimeSeconds();
			}
		}
	}

}
void ATeleShootCharacter::Fire3DPressed() {
	MouseClicked = true;
}

void ATeleShootCharacter::Fire3DRelease() {
	MouseClicked = false;

	UWorld* const World = GetWorld();
	if (World == NULL || World->GetTimeSeconds() < ShotTime + FireRate || !CanMove || !GunUpgrades[3])
		return;
	FVector MousePosition;
	FVector MouseDirection;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		if (PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection)) {
			if (ProjectileClass != NULL) {

				FHitResult Hit(ForceInit);

				FVector TraceStartLocation = SideViewCameraComponent->GetComponentLocation();
				FVector TraceEndLocation = MousePosition + 12000 * MouseDirection;

				FCollisionQueryParams TraceParams(FName(TEXT("CameraTrace")), true, this);
				TraceParams.bTraceAsyncScene = true;
				TraceParams.bReturnPhysicalMaterial = true;
				TraceParams.AddIgnoredActor(this);
				TraceParams.AddIgnoredActor(GetOwner());

				if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStartLocation, TraceEndLocation, ECC_GameTraceChannel2, TraceParams)) {
					FVector EndLocation = Hit.ImpactPoint;
					FVector loc(GetActorLocation());
					FVector projectile(EndLocation - loc);
					FRotator SpawnRotation = projectile.Rotation();
					SpawnRotation.Normalize();
					projectile.Normalize();
					FVector SpawnLocation = loc + projectile * GunOffset;
					AProjectile* bullet = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
					bullet->SetSpeed(FireSpeed);
					FireSpeed = OriginalFireSpeed;
					ShotTime = World->GetTimeSeconds();
				}
			}
		}
	}
}
