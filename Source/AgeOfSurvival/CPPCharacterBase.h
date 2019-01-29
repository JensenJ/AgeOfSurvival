// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPPComponentAttributes.h"
#include "CPPCharacterBase.generated.h"

UCLASS()
class AGEOFSURVIVAL_API ACPPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPPCharacterBase();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	//class UCPPComponentAttributes* AttributesComponent;

	UFUNCTION(BlueprintCallable, Category = Attributes)
	virtual void Die();

	//UFUNCTION(BlueprintCallable, Category = Attributes)
	//virtual UCPPComponentAttributes* GetAttributesComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bCanMove = true;
};


