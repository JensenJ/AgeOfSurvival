// Copyright Age of Survival 2019.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "CPPCharacterPlayer.h"
#include "CPPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFSURVIVAL_API ACPPPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ACPPPlayerController();

	//UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	//void CreateNewHud();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Camera Components in order of parentage
	//UPROPERTY(EditAnywhere, Category = Camera)
	//class USceneComponent* LocalSceneComponent = nullptr;

	//UPROPERTY(EditAnywhere, Category = Camera)
	//class USphereComponent* LocalSphereComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* LocalSpringArmComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* LocalCameraComponent = nullptr;

	// Reference to CameraPawn class
	ACPPCharacterPlayer* PlayerCharacter = nullptr;

	// Function to get controlled pawn
	ACPPCharacterPlayer* GetControlledPawn() const;

	// Input methods
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent);
	void InputMoveCameraForward(float value);
	void InputMoveCameraRight(float value);
	void InputCameraX(float value);
	void InputCameraY(float value);
	void InputZoomIn();
	void InputZoomOut();
	void InputResetPan();
	void InputResetZoom();

	// Camera Calculation Functions
	float CalculateMovementSpeed();
	FTransform MovementX(float AxisValue, float MoveSensitivity, float SpeedMultiplier);
	FTransform MovementY(float AxisValue, float MoveSensitivity, float SpeedMultiplier);
	FRotator PanX(float AxisValue, float PanSensitiviy);
	FRotator PanY(float AxisValue, float PanSensitivity);

	//Camera Variables
	bool bCameraMoveable = true;
	float MovementSpeed = 0.0f;
	float MovementSpeedMin = 5.0f;
	float MovementSpeedMax = 20.0f;
	float FastMoveMultiplier = 1.0f;
	float PanSensitivity = 5.0f;
	float ZoomSensitivity = 50.0f;
	float MinArmDistance = 500.0f;
	float MaxArmDistance = 2500.0f;

};