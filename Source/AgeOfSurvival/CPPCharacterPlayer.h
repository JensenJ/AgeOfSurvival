// Copyright Age of Survival 2019.

#pragma once

#include "CoreMinimal.h"
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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Third Person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera;

	UFUNCTION(BlueprintImplementableEvent, Category = Camera)
	void SwitchPOV(bool bIsFirstPerson);

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
	bool bIsWalking = false;

protected:
	//Movement
	void InputMoveForward(float Value);
	void InputMoveRight(float Value);
	void InputTurnAtRate(float Rate);
	void InputLookUpAtRate(float Rate);

	//Zoom
	void InputZoomIn();
	void InputZoomOut();

	//Walking
	void ToggleWalk();
	float BaseMovementRate = 1.0f;

	//Camera
	float targetLength = 300.0f;
	float POVSwitch = 50.0f;
	float MaxZoom = 500.0f;
	float MinZoom = 10.0f;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns ThirdPersonCamera subobject **/
	FORCEINLINE class UCameraComponent* GetThirdPersonCamera() const { return ThirdPersonCamera; }
};
