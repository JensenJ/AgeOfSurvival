// Copyright Age of Survival 2019.

#include "CPPGameModeBase.h"
#include "CPPCameraPawn.h"
#include "CPPGameStateBase.h"
#include "CPPPlayerController.h"
#include "CPPPlayerState.h"

ACPPGameModeBase::ACPPGameModeBase() {
	PlayerControllerClass = ACPPPlayerController::StaticClass();
	DefaultPawnClass = ACPPCameraPawn::StaticClass();
	PlayerStateClass = ACPPPlayerState::StaticClass();
	GameStateClass = ACPPGameStateBase::StaticClass();
}