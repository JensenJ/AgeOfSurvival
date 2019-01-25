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

void UCPPComponentAttributes::DamageHealth(int32 amount) {
	if (amount <= 0) { // Makes sure negatives are not entered
		return;
	}

	if (bIsInvincibleHealth) {
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

void UCPPComponentAttributes::HealHealth(int32 amount) {
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

void UCPPComponentAttributes::SetInvincibleHealth(bool invincible) {
	bIsInvincibleHealth = invincible;
}

void UCPPComponentAttributes::DamageStamina(int32 amount) {
	if (amount <= 0) { // Makes sure negatives are not entered
		return;
	}

	if (bIsInvincibleStamina) {
		return;
	}

	if ((stamina - amount) < 0) {
		stamina = 0;
	}
	else {
		stamina -= amount;
	}
}

void UCPPComponentAttributes::HealStamina(int32 amount) {
	if (amount <= 0) { //Makes sure negatives are not entered
		return;
	}

	if ((health + amount) > maxHealth) {
		health = maxHealth;
	}
	else {
		health += amount;
	}
}

int32 UCPPComponentAttributes::GetStamina() {
	return stamina;
}

void UCPPComponentAttributes::SetMaxStamina(int32 amount) {
	maxStamina = amount;
}

int32 UCPPComponentAttributes::GetMaxStamina() {
	return maxStamina;
}

void UCPPComponentAttributes::SetInvincibleStamina(bool invincible) {
	bIsInvincibleStamina = invincible;
}