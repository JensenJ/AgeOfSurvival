// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPCharacterBase.h"
#include "Engine.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPPComponentAttributes.h"

// Sets default values
ACPPCharacterBase::ACPPCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	//AttributesComponent = CreateDefaultSubobject<UCPPComponentAttributes>(TEXT("AttributesComponent"));

	//if (!AttributesComponent) {
	//	UE_LOG(LogTemp, Error, TEXT("AttributeComponent not found."))
	//}
}

// Called when the game starts or when spawned
void ACPPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPPCharacterBase::Die() {
	bCanMove = false;
}

//UCPPComponentAttributes* ACPPCharacterBase::GetAttributesComponent() const {
//	return AttributesComponent;
//}