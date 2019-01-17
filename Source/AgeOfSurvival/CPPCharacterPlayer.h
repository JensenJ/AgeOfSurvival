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
	// Sets default values for this pawn's properties
	ACPPCharacterPlayer(const FObjectInitializer& ObjectInit);

	USpringArmComponent* GetSpringArmComponent();
	UCameraComponent* GetCameraComponent();

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* CameraComponent = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
