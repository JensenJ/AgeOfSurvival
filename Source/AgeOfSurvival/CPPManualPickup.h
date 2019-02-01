// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPInteractable.h"
#include "CPPManualPickup.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFSURVIVAL_API ACPPManualPickup : public ACPPInteractable
{
	GENERATED_BODY()
	
public:
	ACPPManualPickup();

	void Interact_Implementation(APlayerController* controller) override;

protected: 
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
};
