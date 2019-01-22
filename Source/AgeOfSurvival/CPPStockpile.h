// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPStockpile.generated.h"

UENUM(BlueprintType) 
enum class EMaterial : uint8 {
	ENone		UMETA(DisplayName = "None"),
	EWood		UMETA(DisplayName = "Wood"),
	EStone		UMETA(DisplayName = "Stone")
	//TODO add more materials
};

UCLASS()
class AGEOFSURVIVAL_API ACPPStockpile : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ACPPStockpile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stockpile)
	int32 MaxWoodAmount = 6;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stockpile)
	int32 MaxStoneAmount = 12;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stockpile)
	EMaterial MaterialType = EMaterial::ENone;

	UFUNCTION(BlueprintCallable, Category = Stockpile)
	void SetMaterialAmount(int32 amount);

	UFUNCTION(BlueprintPure, Category = Stockpile)
	int32 GetMaterialAmount();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32 MaxMaterialAmount;
	int32 CurrentMaterialAmount = 0;
};
