// Copyright Age of Survival 2019.

#include "CPPCharacterPlayer.h"

#include "UObject/UObjectGlobals.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"

ACPPCharacterPlayer::ACPPCharacterPlayer(const class FObjectInitializer& ObjectInit)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	///////////////////////////////////////////////////
	//////////// Camera Component Setup ///////////////
	///////////////////////////////////////////////////

	// Set controller rotator variables
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Instantiate all components
	//SceneComponent = ObjectInit.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComponent"), false);
	//SphereComponent = ObjectInit.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComponent"), false);
	SpringArmComponent = ObjectInit.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("SpringArmComponent"), false);
	CameraComponent = ObjectInit.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CameraComponent"), false);

	if (!SpringArmComponent) {
		UE_LOG(LogTemp, Error, TEXT("PECameraPawn: Spring Arm Component not found::PointerProtection"));
		return;
	}
	if (!CameraComponent) {
		UE_LOG(LogTemp, Error, TEXT("PECameraPawn: Camera Component not found::PointerProtection"));
		return;
	}

	// Setup scene component
	//SceneComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//this->SetRootComponent(SceneComponent);
	//SceneComponent->RelativeLocation = FVector(0.0f, 0.0f, 50.0f);

	// Setup sphere component
	//SphereComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//SphereComponent->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	//SphereComponent->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	//SphereComponent->bHiddenInGame = true;

	// Setup spring arm component
	SpringArmComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComponent->bAbsoluteRotation = false;
	SpringArmComponent->TargetArmLength = 500.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	SpringArmComponent->RelativeRotation = FRotator(0.0, 0.0f, 0.0f);

	// Setup camera component
	CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void ACPPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	// Set Rotation of pawn
	FRotator CamRotation = FRotator(0.0f, 0.0f, 0.0f);
	SetActorRotation(CamRotation);
}

// Called every frame
void ACPPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPPCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Getters for all camera components
USpringArmComponent* ACPPCharacterPlayer::GetSpringArmComponent() {
	return SpringArmComponent;
}
UCameraComponent* ACPPCharacterPlayer::GetCameraComponent() {
	return CameraComponent;
}