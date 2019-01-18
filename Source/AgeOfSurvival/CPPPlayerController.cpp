// Copyright Age of Survival 2019.

#include "CPPPlayerController.h"
#include "CPPCharacterPlayer.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

ACPPPlayerController::ACPPPlayerController() {

}

// Retrieves the PECameraPawn class
ACPPCharacterPlayer* ACPPPlayerController::GetControlledPawn() const {
	return Cast<ACPPCharacterPlayer>(GetPawn());
}

void ACPPPlayerController::BeginPlay() {

}

///////////////////////////////////////////////////
///////////////// Player Input ////////////////////
///////////////////////////////////////////////////

void ACPPPlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent) {

}