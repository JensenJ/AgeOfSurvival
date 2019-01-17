// Copyright Age of Survival 2019.

#include "CPPPlayerController.h"
#include "CPPCharacterPlayer.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

ACPPPlayerController::ACPPPlayerController() {
	// Allows mouse cursor to interact with buttons and screen and be displayed
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

// Retrieves the PECameraPawn class
ACPPCharacterPlayer* ACPPPlayerController::GetControlledPawn() const {
	return Cast<ACPPCharacterPlayer>(GetPawn());
}

void ACPPPlayerController::BeginPlay() {
	SetupPlayerInputComponent(InputComponent);

	// Get camera pawn class
	PlayerCharacter = GetControlledPawn();
	// Check if camera pawn was found
	if (!PlayerCharacter) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: CameraPawn Class not found"));
		return;
	}
	//Declare local components for easy access
	//LocalSceneComponent = PlayerCharacter->GetSceneComponent();
	//LocalSphereComponent = PlayerCharacter->GetSphereComponent();
	LocalSpringArmComponent = PlayerCharacter->GetSpringArmComponent();
	LocalCameraComponent = PlayerCharacter->GetCameraComponent();

	//CreateNewHud();
}

///////////////////////////////////////////////////
///////////////// Player Input ////////////////////
///////////////////////////////////////////////////

void ACPPPlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	//Check for input component
	EnableInput(this);
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Input Component not found::PointerProtection"));
		return;
	}
	// Binding actions
	//InputComponent->BindAction("TogglePan", IE_Pressed, this, &ACPPPlayerController::InputTogglePan);
	//InputComponent->BindAction("FastMove", IE_Pressed, this, &ACPPPlayerController::InputToggleFastMove);
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ACPPPlayerController::InputZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ACPPPlayerController::InputZoomOut);
	InputComponent->BindAction("ResetPan", IE_Pressed, this, &ACPPPlayerController::InputResetPan);
	InputComponent->BindAction("ResetZoom", IE_Pressed, this, &ACPPPlayerController::InputResetZoom);
	// Binding axis
	InputComponent->BindAxis("MoveForward", this, &ACPPPlayerController::InputMoveCameraForward);
	InputComponent->BindAxis("MoveRight", this, &ACPPPlayerController::InputMoveCameraRight);
	InputComponent->BindAxis("XMovement", this, &ACPPPlayerController::InputCameraX);
	InputComponent->BindAxis("YMovement", this, &ACPPPlayerController::InputCameraY);
}

// Sets pawn's transform on X axis based on calculated movement speed
void ACPPPlayerController::InputMoveCameraForward(float AxisValue) {
	if (!PlayerCharacter) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	MovementSpeed = CalculateMovementSpeed();
	PlayerCharacter->SetActorTransform(MovementX(MovementSpeed, FastMoveMultiplier, AxisValue));
}

// Sets pawn's transform on Y axis based on calculated movement speed
void ACPPPlayerController::InputMoveCameraRight(float AxisValue) {
	if (!PlayerCharacter) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	MovementSpeed = CalculateMovementSpeed();
	PlayerCharacter->SetActorTransform(MovementY(AxisValue, MovementSpeed, FastMoveMultiplier));
}

// Gets input from X movement on mouse or game pad
void ACPPPlayerController::InputCameraX(float AxisValue) {
	if (!PlayerCharacter) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not

	PlayerCharacter->SetActorRotation(PanX(AxisValue, PanSensitivity));
	//float XDeltaSpeed, YDeltaSpeed; // Declaring OUT parameters
	//FVector LocalOffsetVector = UKismetMathLibrary::MakeVector(XDeltaSpeed, YDeltaSpeed, 0.0f); //Making vector based on this
	//PlayerCharacter->AddActorLocalOffset(LocalOffsetVector, true); // Adding offset from vector to camera pawn
}

// Gets input from Y movement on mouse or game pad
void ACPPPlayerController::InputCameraY(float AxisValue) {
	if (!PlayerCharacter) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not

	PlayerCharacter->SetActorRotation(PanY(AxisValue, PanSensitivity));
	//float XDeltaSpeed, YDeltaSpeed; // Declaring OUT parameters
	//FVector LocalOffsetVector = UKismetMathLibrary::MakeVector(XDeltaSpeed, YDeltaSpeed, 0.0f); //Making vector based on this
	//PlayerCharacter->AddActorLocalOffset(LocalOffsetVector, true); // Adding offset from vector to camera pawn
}

// Sets new zoom in value
void ACPPPlayerController::InputZoomIn() {
	if (!PlayerCharacter) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	float ZoomValue = LocalSpringArmComponent->TargetArmLength - ZoomSensitivity; //Takes zoom sensitivity away from target arm length
	ZoomValue = FMath::Clamp(ZoomValue, MinArmDistance, MaxArmDistance); // Clamps values between min and max
	LocalSpringArmComponent->TargetArmLength = ZoomValue; // Sets new spring arm length
}
// Sets new zoom out value
void ACPPPlayerController::InputZoomOut() {
	if (!PlayerCharacter) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	float ZoomValue = LocalSpringArmComponent->TargetArmLength + ZoomSensitivity; // Adds zoom sensitivity to target arm length
	ZoomValue = FMath::Clamp(ZoomValue, MinArmDistance, MaxArmDistance); // Clamps values between min and max
	LocalSpringArmComponent->TargetArmLength = ZoomValue; // Sets new spring arm length
}
// Resets panning
void ACPPPlayerController::InputResetPan() {
	if (!PlayerCharacter) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	PlayerCharacter->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); // Rotates back to original rotation
}
// Resets zoom
void ACPPPlayerController::InputResetZoom() {
	if (!PlayerCharacter) return; // Checks for camera pawn
	if (!bCameraMoveable) return; // Checks whether camera is move able or not
	LocalSpringArmComponent->TargetArmLength = 500; // Sets spring arm length to default value
}
///////////////////////////////////////////////////
/////////////// Camera Calculations ///////////////
///////////////////////////////////////////////////

// Calculates movement speed
float ACPPPlayerController::CalculateMovementSpeed() {
	if (!LocalSpringArmComponent) { //Checks for spring arm component
		UE_LOG(LogTemp, Error, TEXT("PEPlayerController: Calculation of movement speed X failed::LocalSpringArmComponent"));
		return 100.0f; // returns 100 as a default
	}
	float LocalMovementSpeed = LocalSpringArmComponent->TargetArmLength / 100; // divides target arm length by 100 to get value to clamp
	LocalMovementSpeed = FMath::Clamp(LocalMovementSpeed, MovementSpeedMin, MovementSpeedMax); // clamp value between min and max
	return LocalMovementSpeed; // Returns local movement speed
}

// Calculates movement on X axis
FTransform ACPPPlayerController::MovementX(float AxisValue, float MovementSpeed, float SpeedMultiplier) {
	MovementSpeed = (MovementSpeed * SpeedMultiplier) * AxisValue; // Works out base movement speed

	FVector ForwardMovementVector = FVector(MovementSpeed, 0.0f, 0.0f); // Constructs a new vector based on new value
	FTransform CameraPawnTransform = PlayerCharacter->GetActorTransform(); // Gets camera pawn transform
	FVector TransformDirection = UKismetMathLibrary::TransformDirection(CameraPawnTransform, ForwardMovementVector); // Creates a transform direction from vectors and transform
	// Declaring OUT variables
	FVector OutTransform;
	FRotator OutRotator;
	FVector OutScale;
	UKismetMathLibrary::BreakTransform(CameraPawnTransform, OutTransform, OutRotator, OutScale); //Breaks transform
	FVector AddedVector = TransformDirection + OutTransform; // Adds transforms together
	FTransform FinalTransform = UKismetMathLibrary::MakeTransform(AddedVector, OutRotator, OutScale); // Creates a new transform with inputted values
	return FinalTransform; // Returns final transform
}

// Calculates movement on Y axis
FTransform ACPPPlayerController::MovementY(float AxisValue, float MovementSpeed, float SpeedMultiplier) {
	MovementSpeed = (MovementSpeed * SpeedMultiplier) * AxisValue; // Works out base movement speed

	FVector ForwardMovementVector = FVector(0.0f, MovementSpeed, 0.0f); // Constructs a new vector based on new values
	FTransform CameraPawnTransform = PlayerCharacter->GetActorTransform(); // Gets camera pawn transform
	FVector TransformDirection = UKismetMathLibrary::TransformDirection(CameraPawnTransform, ForwardMovementVector); // Creates a transform direction from vectors and transform
	// Declaring OUT variables
	FVector OutTransform;
	FRotator OutRotator;
	FVector OutScale;
	UKismetMathLibrary::BreakTransform(CameraPawnTransform, OutTransform, OutRotator, OutScale); //Breaks transform
	FVector AddedVector = TransformDirection + OutTransform; // Adds transforms together
	float x, y, z; // Declaring more OUT variables
	UKismetMathLibrary::BreakVector(AddedVector, x, y, z);
	z = 100; // Forces Z axis to be 100, preventing bug of changing height constantly
	FVector FinalVector = UKismetMathLibrary::MakeVector(x, y, z); // Constructs a new vector based on new values
	FTransform FinalTransform = UKismetMathLibrary::MakeTransform(FinalVector, OutRotator, OutScale); // Creates a new transform with inputted values
	return FinalTransform; // Returns final transform
}

// Calculations for panning on the X axis
FRotator ACPPPlayerController::PanX(float AxisValue, float PanSensitivity) {
	PanSensitivity = PanSensitivity * AxisValue; // Works out pan sensitivity
	FRotator CameraPawnRot = PlayerCharacter->GetActorRotation(); // Gets camera pawn rotation
	float RotatorRoll, RotatorPitch, RotatorYaw; // Declaring OUT variables
	UKismetMathLibrary::BreakRotator(CameraPawnRot, RotatorRoll, RotatorPitch, RotatorYaw); // Breaking rotator
	PanSensitivity = PanSensitivity + RotatorYaw; // Adds yaw to pan sensitivity
	FRotator FinalRotator = UKismetMathLibrary::MakeRotator(RotatorRoll, RotatorPitch, PanSensitivity); // Constructs final rotator based on new values
	return FinalRotator; // Returns final rotator
}

// Calculations for panning on the Y axis
FRotator ACPPPlayerController::PanY(float AxisValue, float PanSensitivity) {
	PanSensitivity = PanSensitivity * AxisValue; // Works out pan sensitivity
	FRotator CameraPawnRot = PlayerCharacter->GetActorRotation(); // Gets camera pawn rotation
	float RotatorRoll, RotatorPitch, RotatorYaw; // Declaring OUT variables
	UKismetMathLibrary::BreakRotator(CameraPawnRot, RotatorRoll, RotatorPitch, RotatorYaw); // Breaking rotator
	PanSensitivity = PanSensitivity + RotatorPitch; // Adds yaw to pan sensitivity
	PanSensitivity = FMath::Clamp(PanSensitivity, -80.0f, 0.0f); // Clamps values to avoid rotating under the map or above the pivot point, which would cause inverted controls
	FRotator FinalRotator = UKismetMathLibrary::MakeRotator(RotatorRoll, PanSensitivity, RotatorYaw); // Constructs final rotator based on new values
	return FinalRotator; // Returns final rotator
}