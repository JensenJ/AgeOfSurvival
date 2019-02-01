// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPInteractable.generated.h"

UCLASS()
class AGEOFSURVIVAL_API ACPPInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPPInteractable();

	UFUNCTION(BlueprintNativeEvent)
	void Interact(APlayerController* controller);
	
	virtual void Interact_Implementation(APlayerController* controller);

	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	FString Action;

	UFUNCTION(BlueprintCallable, Category = Pickup)
	FString GetInteractText() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
