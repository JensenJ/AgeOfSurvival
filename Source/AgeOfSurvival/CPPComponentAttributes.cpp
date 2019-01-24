// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPComponentAttributes.h"

// Sets default values for this component's properties
UCPPComponentAttributes::UCPPComponentAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPPComponentAttributes::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCPPComponentAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCPPComponentAttributes::Damage(int32 amount) {
	if (amount <= 0) { // Makes sure negatives are not entered
		return;
	}

	if ((health - amount) < 0) {
		health = 0;
	}
	else {
		health -= amount;
	}

	if (health <= 0) {
		bIsDead = true;
	}
}

void UCPPComponentAttributes::Heal(int32 amount) {
	if (amount <= 0) { // Makes sure negatives are not entered
		return;
	}

	if ((health + amount) > maxHealth) {
		health = maxHealth;
	}
	else {
		health += amount;
	}
}

int32 UCPPComponentAttributes::GetHealth() {
	return health;
}

void UCPPComponentAttributes::SetMaxHealth(int32 amount) {
	maxHealth = amount;
}

int32 UCPPComponentAttributes::GetMaxHealth() {
	return maxHealth;
}