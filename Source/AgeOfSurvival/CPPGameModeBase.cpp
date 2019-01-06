// Copyright Age of Survival 2019.

#include "CPPGameModeBase.h"
#include "CPPCameraPawn.h"
#include "CPPGameStateBase.h"
#include "CPPPlayerController.h"
#include "CPPPlayerState.h"

//Set default classes
ACPPGameModeBase::ACPPGameModeBase() {
	PlayerControllerClass = ACPPPlayerController::StaticClass();
	DefaultPawnClass = ACPPCameraPawn::StaticClass();
	PlayerStateClass = ACPPPlayerState::StaticClass();
	GameStateClass = ACPPGameStateBase::StaticClass();
}