// Copyright Age of Survival 2019.

#pragma once

#include "Engine.h"
#include "CPPCharacterBase.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "CPPCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFSURVIVAL_API ACPPCharacterPlayer : public ACPPCharacterBase
{
	GENERATED_BODY()
public:

	virtual void Tick(float DeltaTime) override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Third Person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera;

	UFUNCTION(BlueprintImplementableEvent, Category = Camera)
	void SwitchPOV(bool bIsFirstPerson);

	void Die() override;

	ACPPCharacterPlayer();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 1.0f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseZoomRate = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float WalkingSpeed = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float RunningSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float CrouchingSpeed = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsWalking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsCrouching = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	bool bIsFirstPerson = false;

protected:
	//Movement
	void InputMoveForward(float Value);
	void InputMoveRight(float Value);
	void InputTurnAtRate(float Rate);
	void InputLookUpAtRate(float Rate);

	float BaseMovementRate = 1.0f;
	void ToggleWalk();
	void ToggleCrouch();

	//Zoom
	void InputZoomIn();
	void InputZoomOut();


	//Camera
	float targetLength = 300.0f;
	float POVSwitch = 50.0f;
	float MaxZoom = 500.0f;
	float MinZoom = 10.0f;
	bool bCanCameraMove = true;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void CheckForInteractables();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns ThirdPersonCamera subobject **/
	FORCEINLINE class UCameraComponent* GetThirdPersonCamera() const { return ThirdPersonCamera; }
};
