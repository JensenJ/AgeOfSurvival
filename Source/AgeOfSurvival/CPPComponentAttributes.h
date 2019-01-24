// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPPComponentAttributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGEOFSURVIVAL_API UCPPComponentAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPPComponentAttributes();

	//Health
	UFUNCTION(BlueprintCallable, Category = Health)
	void Damage(int32 amount);

	UFUNCTION(BlueprintCallable, Category = Health)
	void Heal(int32 amount);

	UFUNCTION(BlueprintPure, Category = Health)
	int32 GetHealth();

	UFUNCTION(BlueprintCallable, Category = Health)
	void SetMaxHealth(int32 amount);

	UFUNCTION(BlueprintPure, Category = Health)
	int32 GetMaxHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 health = 100;
	int32 maxHealth = 100;
	bool bIsDead = false;
};
