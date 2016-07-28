// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "HoldableBox.h"
#include "GunUpgrade.h"

#include "TeleShootCharacter.generated.h"

UCLASS(config = Game)
class ATeleShootCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Side view camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* BoxSpring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HoldBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractBox;


protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	virtual void Fire2DPressed();
	virtual void Fire2DRelease();

	virtual void Fire3DPressed();
	virtual void Fire3DRelease();

	void Duck();
	void Unduck();

	void WalkOn();
	void WalkOff();

	virtual void TeleJump();
	virtual void Interact();
	virtual void Tick(float DeltaTime);

	virtual void BeginPlay();

public:
	ATeleShootCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UBoxComponent* GetInteractBox() const { return InteractBox; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shooting)
		float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting)
		float offset;

	void SwitchLocation(FVector Location, bool CanJumpAgain);

	void UpgradeGun(UpgradeType Upgrade);

	bool HasGunUpgrade(UpgradeType Upgrade);

	void UpdateBoxPosition();

	void EndLevel();

private:
	void CheckMousePosition(FVector ActorLocation, FVector CameraLocation, float DeltaTime);
	void DoneSlide();


	AHoldableBox* HoldingBox;
	FTimerHandle SlideTimer;
	FVector TargetLocation;
	FVector LPad, RPad, TPad, BPad;
	FVector OriginalCameraPosition;
	float SprintSpeed;
	float WalkSpeed;
	float OriginalBoomLength;
	float OriginalFireSpeed;
	float OriginalYAxis;
	float CurrentYAxis;
	float ShotTime;
	float FireSpeed;
	float FireSpeedMax;
	float FireSpeedRate;
	float CameraRate;
	float CameraMoveMax;
	float CameraMax;
	float CameraMoveRate;
	float CameraReturnRate;
	float DoubleJumpHeight;
	bool MouseClicked;
	bool MouseLeftCenter;
	bool JustTeleported;
	bool AtPeak;
	bool IsSliding;
	bool TryingToUncrouch;
	bool CanMove;
	bool GunUpgrades[6] = {false}; //{TeleportTo, BringTo, Switch, ThreeD, TimeSlow, Charge}
};
