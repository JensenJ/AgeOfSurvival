// Copyright Age of Survival 2019.

#include "CPPGameModeBase.h"
#include "CPPGameStateBase.h"
#include "CPPPlayerController.h"
#include "CPPPlayerState.h"
#include "UObject/ConstructorHelpers.h"

//Set default classes
ACPPGameModeBase::ACPPGameModeBase() {
	PlayerControllerClass = ACPPPlayerController::StaticClass();
	PlayerStateClass = ACPPPlayerState::StaticClass();
	GameStateClass = ACPPGameStateBase::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BPCharacterPlayer"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}