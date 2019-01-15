// Copyright Age of Survival 2019.

#pragma once

#include "CoreMinimal.h"
#include "CPPCharacterBase.h"
#include "CPPCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFSURVIVAL_API ACPPCharacterPlayer : public ACPPCharacterBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SensitivityMultiplier = 1.0f;
protected:
	void MoveRight(float value);
	void MoveForward(float value);
	
	void MouseX(float value);
	void MouseY(float value);

	void Jump();

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:
	ACPPCharacterPlayer();

	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
