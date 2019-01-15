// Copyright Age of Survival 2019.

#include "CPPCharacterPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACPPCharacterPlayer::ACPPCharacterPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 0.f, 0.f);

	// Create a camera and attach to boom
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

}

void ACPPCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACPPCharacterPlayer::Jump);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPPCharacterPlayer::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPPCharacterPlayer::MoveForward);

	PlayerInputComponent->BindAxis("MouseX", this, &ACPPCharacterPlayer::MouseX);
	PlayerInputComponent->BindAxis("MouseY", this, &ACPPCharacterPlayer::MouseY);
}

void ACPPCharacterPlayer::MoveForward(float Value) {
	Value *= MovementMultiplier;
	AddMovementInput(FVector(-1.0f, 0.0f, 0.0f), Value);
}

void ACPPCharacterPlayer::MoveRight(float Value) {
	Value *= MovementMultiplier;
	AddMovementInput(FVector(0.0f, -1.0f, 0.0f), Value);
}

void ACPPCharacterPlayer::MouseX(float Value) {
	Value *= SensitivityMultiplier;
	UE_LOG(LogTemp, Error, TEXT("Test: %f"), Value)
	AddControllerYawInput(Value);
}

void ACPPCharacterPlayer::MouseY(float Value) {
	Value *= SensitivityMultiplier;
	AddControllerPitchInput(Value);
}