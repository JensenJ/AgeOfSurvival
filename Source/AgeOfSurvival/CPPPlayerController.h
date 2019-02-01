// Copyright Age of Survival 2019.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "CPPCharacterPlayer.h"

#include "CPPInteractable.h"
#include "CPPInventoryItem.h"

#include "CPPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFSURVIVAL_API ACPPPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ACPPPlayerController();

	UFUNCTION(BlueprintImplementableEvent)
	void ReloadInventory();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetInventoryWeight();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddItemToInventoryByID(FName ID);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ACPPInteractable* CurrentInteractable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 InventoryWeightLimit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 InventorySlotLimit;
protected:

	void Interact();

private:
	// Input methods
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent);

};