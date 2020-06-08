// Fill out your copyright notice in the Description page of Project Settings.

#include "NyoomCharacter.h"

// Sets default values
ANyoomCharacter::ANyoomCharacter(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer.SetDefaultSubobjectClass<UNyoomMovementComponent>(ACharacter::CharacterMovementComponentName)) {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->playerInput = FVector2D();
}

// Called when the game starts or when spawned
void ANyoomCharacter::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ANyoomCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANyoomCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent) {
	check(playerInputComponent);

	// Handle movement inputs
    playerInputComponent->BindAxis("Forward", this, &ANyoomCharacter::InputForward);
    playerInputComponent->BindAxis("Strafe", this, &ANyoomCharacter::InputStrafe);

    // Handle mouse inputs
    playerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    playerInputComponent->BindAxis("Right", this, &APawn::AddControllerYawInput);
}

void ANyoomCharacter::PostInitializeComponents() {
    Super::PostInitializeComponents();

    this->movementComponent = Cast<UNyoomMovementComponent>(Super::GetMovementComponent());
}

void ANyoomCharacter::InputForward(float val) {
    this->playerInput.X = val;
}

void ANyoomCharacter::InputStrafe(float val) {
    this->playerInput.Y = val;
}

void ANyoomCharacter::CalcFriction() {
    FVector dir;
    float speed;

    this->velocity.ToDirectionAndLength(dir, speed);

}