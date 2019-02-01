// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPManualPickup.h"
#include "CPPPlayerController.h"

ACPPManualPickup::ACPPManualPickup() {
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);

	ItemID = FName("No ID");

	Super::Name = "Item";
	Super::Action = "Pickup";
}

void ACPPManualPickup::Interact_Implementation(APlayerController* controller) {
	Super::Interact_Implementation(controller);

	ACPPPlayerController* playerController = Cast<ACPPPlayerController>(controller);
	if (playerController->AddItemToInventoryByID(ItemID)) {
		Destroy();
	}
}