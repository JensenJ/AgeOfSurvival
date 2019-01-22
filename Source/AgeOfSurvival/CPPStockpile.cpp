// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPStockpile.h"

// Sets default values
ACPPStockpile::ACPPStockpile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPPStockpile::BeginPlay()
{
	Super::BeginPlay();
	if (MaterialType == EMaterial::EWood) {
		MaxMaterialAmount = MaxWoodAmount;
	}
	else if (MaterialType == EMaterial::EStone) {
		MaxMaterialAmount = MaxStoneAmount;
	}
	else {
		MaxMaterialAmount = 8;
	}
}

// Called every frame
void ACPPStockpile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPPStockpile::SetMaterialAmount(int32 amount) {
	if (amount > MaxMaterialAmount) {
		UE_LOG(LogTemp, Error, TEXT("Stockpile::More material has been added than the stockpile can support"))
	}
	else if (amount < 0) {
		UE_LOG(LogTemp, Error, TEXT("Stockpile::Tried removing item from stockpile which does not exist"))
	}
	else {
		CurrentMaterialAmount = amount;
	}
}

int32 ACPPStockpile::GetMaterialAmount() {
	return CurrentMaterialAmount;
}