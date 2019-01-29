// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPComponentAttributes.h"
#include "CPPCharacterBase.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UCPPComponentAttributes::UCPPComponentAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCPPComponentAttributes::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()) {
		UE_LOG(LogTemp, Error, TEXT("PointerPrevention::Owner"))
		return;
	}


	character = GetOwner();
	if (!character) {
		UE_LOG(LogTemp, Error, TEXT("PointerPrevention::Character"))
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("NAME: %s"), *character->GetName())
}


// Called every frame
void UCPPComponentAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UCPPComponentAttributes::DecreaseAttributeValue(int32 amount, EAttributeEnum attribute) {
	if (bIsDead) {
		return;
	}
	if (amount <= 0) { // Makes sure negatives are not entered
		return;
	}

	if (attribute == EAttributeEnum::EHealth) {
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
			
			ACPPCharacterBase* characterBase = dynamic_cast<ACPPCharacterBase*>(character);
			if (!characterBase) { return; }
			characterBase->Die();
			
		}
	}
	else if (attribute == EAttributeEnum::EStamina) {
		if (bIsInvincibleStamina) {
			return;
		}

		if ((stamina - amount) < 0) {
			stamina = 0;
		}
		else {
			stamina -= amount;
		}

		if (stamina <= 0) {
			bIsExhausted = true;
		}
	}
	else if (attribute == EAttributeEnum::EHunger) {
		if (bIsInvincibleHunger) {
			return;
		}

		if ((hunger - amount) < 0) {
			hunger = 0;
		}
		else {
			hunger -= amount;
		}

		if (hunger <= 0) {
			bIsStarving = true;
		}
	}
	else if (attribute == EAttributeEnum::EThirst) {
		if (bIsInvincibleThirst) {
			return;
		}

		if ((thirst - amount) < 0) {
			thirst = 0;
		}
		else {
			thirst -= amount;
		}
		
		if (thirst <= 0) {
			bIsDehydrated = true;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ComponentAttribute::Attribute not identified."))
	}
}

void UCPPComponentAttributes::IncreaseAttributeValue(int32 amount, EAttributeEnum attribute) {
	if (bIsDead) {
		return;
	}
	if (amount <= 0) { // Makes sure negatives are not entered
		return;
	}

	if (attribute == EAttributeEnum::EHealth) {
		if ((health + amount) > maxHealth) {
			health = maxHealth;
		}
		else {
			health += amount;
		}
	}
	else if (attribute == EAttributeEnum::EStamina) {
		if ((stamina + amount) > maxStamina) {
			stamina = maxStamina;
		}
		else {
			stamina += amount;
		}
	}
	else if (attribute == EAttributeEnum::EHunger) {
		if ((hunger + amount) > maxHunger) {
			hunger = maxHunger;
		}
		else {
			hunger += amount;
		}
	}
	else if (attribute == EAttributeEnum::EThirst) {
		if ((thirst + amount) > maxThirst) {
			thirst = maxThirst;
		}
		else {
			thirst += amount;
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ComponentAttribute::Attribute not identified."))
	}
}

float UCPPComponentAttributes::GetAttributeValue(EAttributeEnum attribute) {
	if (attribute == EAttributeEnum::EHealth) {
		return health;
	}
	else if (attribute == EAttributeEnum::EStamina) {
		return stamina;
	}
	else if (attribute == EAttributeEnum::EHunger) {
		return hunger;
	}
	else if (attribute == EAttributeEnum::EThirst) {
		return thirst;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ComponentAttribute::Attribute not identified."))
		return 0;
	}
}

void UCPPComponentAttributes::SetMaxAttributeValue(int32 amount, EAttributeEnum attribute) {
	if (attribute == EAttributeEnum::EHealth) {
		maxHealth = amount;
	}
	else if (attribute == EAttributeEnum::EStamina) {
		maxStamina = amount;
	}
	else if (attribute == EAttributeEnum::EHunger) {
		maxHunger = amount;
	}
	else if (attribute == EAttributeEnum::EThirst) {
		maxThirst = amount;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ComponentAttribute::Attribute not identified."))
	}
}

float UCPPComponentAttributes::GetMaxAttributeValue(EAttributeEnum attribute) {
	if (attribute == EAttributeEnum::EHealth) {
		return maxHealth;
	}
	else if (attribute == EAttributeEnum::EStamina) {
		return maxStamina;
	}
	else if (attribute == EAttributeEnum::EHunger) {
		return maxHunger;
	}
	else if (attribute == EAttributeEnum::EThirst) {
		return maxThirst;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ComponentAttribute::Attribute not identified."))
		return 0;
	}
}

void UCPPComponentAttributes::SetInvincibleAttribute(bool invincible, EAttributeEnum attribute) {
	if (attribute == EAttributeEnum::EHealth) {
		bIsInvincibleHealth = invincible;
	}
	else if (attribute == EAttributeEnum::EStamina) {
		bIsInvincibleStamina = invincible;
	}
	else if (attribute == EAttributeEnum::EHunger) {
		bIsInvincibleHunger = invincible;
	}
	else if (attribute == EAttributeEnum::EThirst) {
		bIsInvincibleThirst = invincible;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ComponentAttribute::Attribute not identified."))
	}
}

bool UCPPComponentAttributes::GetAttributeFatal(EAttributeEnum attribute) {
	if (attribute == EAttributeEnum::EHealth) {
		return bIsDead;
	}
	else if (attribute == EAttributeEnum::EStamina) {
		return bIsExhausted;
	}
	else if (attribute == EAttributeEnum::EHunger) {
		return bIsStarving;
	}
	else if (attribute == EAttributeEnum::EThirst) {
		return bIsDehydrated;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ComponentAttribute::Attribute not identified."))
		return false;
	}
}