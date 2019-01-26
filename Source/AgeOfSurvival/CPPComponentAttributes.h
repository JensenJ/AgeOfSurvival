// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPPComponentAttributes.generated.h"


UENUM(BlueprintType)
enum class EAttributeEnum : uint8 {
	EHealth		UMETA(DisplayName = "Health"),
	EStamina	UMETA(DisplayName = "Stamina"),
	EHunger		UMETA(DisplayName = "Hunger"),
	EThirst		UMETA(DisplayName = "Thirst")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGEOFSURVIVAL_API UCPPComponentAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPPComponentAttributes();

	//Health
	UFUNCTION(BlueprintCallable, Category = Health)
	void DecreaseAttributeValue(int32 amount, EAttributeEnum attribute);

	UFUNCTION(BlueprintCallable, Category = Health)
	void IncreaseAttributeValue(int32 amount, EAttributeEnum attribute);

	UFUNCTION(BlueprintPure, Category = Health)
	float GetAttributeValue(EAttributeEnum attribute);

	UFUNCTION(BlueprintCallable, Category = Health)
	void SetMaxAttributeValue(int32 amount, EAttributeEnum attribute);

	UFUNCTION(BlueprintPure, Category = Health)
	float GetMaxAttributeValue(EAttributeEnum attribute);

	UFUNCTION(BlueprintCallable, Category = Health)
	void SetInvincibleAttribute(bool invincible, EAttributeEnum attribute);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//health
	int32 health = 100;
	int32 maxHealth = 100;
	bool bIsDead = false;
	bool bIsInvincibleHealth = false;

	//stamina
	int32 stamina = 100;
	int32 maxStamina = 100;
	bool bIsTired = false;
	bool bIsInvincibleStamina = false;

	//hunger
	int32 hunger = 100;
	int32 maxHunger = 100;
	bool bIsStarving = false;
	bool bIsInvincibleHunger = false;

	//thirst
	int32 thirst = 100;
	int32 maxThirst = 100;
	bool bIsDehydrated = false;
	bool bIsInvincibleThirst = false;
};