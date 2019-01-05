// Copyright Age of Survival 2019.

#include "CPPCameraPawn.h"

// Sets default values
ACPPCameraPawn::ACPPCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPPCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPPCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPPCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

