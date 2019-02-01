// Copyright Age of Survival 2019.

#include "CPPPlayerController.h"

#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

#include "CPPCharacterPlayer.h"
#include "CPPGameStateBase.h"

ACPPPlayerController::ACPPPlayerController() {
	InventorySlotLimit = 50;
	InventoryWeightLimit = 500;
}

void ACPPPlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	InputComponent->BindAction("Interact", IE_Pressed, this, &ACPPPlayerController::Interact);
}

int32 ACPPPlayerController::GetInventoryWeight() {
	int32 weight = 0;
	for (auto& Item : Inventory) {
		weight += Item.Weight;
	}
	return weight;
}

bool ACPPPlayerController::AddItemToInventoryByID(FName ID) {
	ACPPGameStateBase* GameState = Cast<ACPPGameStateBase>(GetWorld()->GetGameState());
	UDataTable* ItemTable = GameState->GetItemDB();
	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	if (ItemToAdd) {
		if (Inventory.Num() < InventorySlotLimit && GetInventoryWeight() + ItemToAdd->Weight <= InventoryWeightLimit) {
			Inventory.Add(*ItemToAdd);
			ReloadInventory();
			return true;
		}
	}
	return false;
}

void ACPPPlayerController::Interact() {
	if (CurrentInteractable) {
		CurrentInteractable->Interact(this);
	}
}