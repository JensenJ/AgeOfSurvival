// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPInteractable.h"

// Sets default values
ACPPInteractable::ACPPInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Name = "Interactable";
	Action = "interact";
}

void ACPPInteractable::Interact_Implementation(APlayerController* controller) {
	return;
}

FString ACPPInteractable::GetInteractText() const {
	return FString::Printf(TEXT("%s: Press F to %s"), *Name, *Action);
}

// Called when the game starts or when spawned
void ACPPInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPPInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

