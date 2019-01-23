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

	/** Camera boom positioning the camera behind the character */


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Camera Components in order of parentage
	//UPROPERTY(EditAnywhere, Category = Camera)
	//class USceneComponent* LocalSceneComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USphereComponent* LocalSphereComponent = nullptr;

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

	void InputMoveForward(float Value);
	void InputMoveRight(float Value);
	void InputTurnAtRate(float Rate);
	void InputLookUpAtRate(float Rate);

	float MoveSpeedMultiplier = 1.0f;
};