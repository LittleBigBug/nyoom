// Fill out your copyright notice in the Description page of Project Settings.

#include "NyoomCharacter.h"

#include "Components/CapsuleComponent.h"

static TAutoConsoleVariable<int32> CVarAutoBunnyHop(TEXT("move.autoBhop"), 1, TEXT("Automatically jump when you hit the ground\n"), ECVF_Default);

// Sets default values
ANyoomCharacter::ANyoomCharacter(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer.SetDefaultSubobjectClass<UNyoomMovementComponent>(ACharacter::CharacterMovementComponentName)) {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Maybe change this to a box collision component
    UCapsuleComponent* capsuleComponent = GetCapsuleComponent();

    capsuleComponent->InitCapsuleSize(30.48f, 68.58f);
    capsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
    capsuleComponent->bReturnMaterialOnMove = true;

    BaseEyeHeight = 54.34f;
}

// Called when the game starts or when spawned
void ANyoomCharacter::BeginPlay() {
	Super::BeginPlay();
}

void ANyoomCharacter::InputForward(float val) {
    if (FMath::IsNearlyZero(val)) {
        return;
    }

    AddMovementInput(GetActorForwardVector(), val);
}

void ANyoomCharacter::InputStrafe(float val) {
    if (FMath::IsNearlyZero(val)) {
        return;
    }

    AddMovementInput(GetActorRightVector(), val);
}

// there MUST BE a better way...
void ANyoomCharacter::InputCrouch() {
    Crouch();
}

void ANyoomCharacter::InputUnCrouch() {
    UnCrouch();
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
    playerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);

    // Handle jumping inputs
    playerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    playerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    // Handle crouching inputs
    playerInputComponent->BindAction("Crouch", IE_Pressed, this, &ANyoomCharacter::InputCrouch);
    playerInputComponent->BindAction("Crouch", IE_Released, this, &ANyoomCharacter::InputUnCrouch);
}

void ANyoomCharacter::PostInitializeComponents() {
    Super::PostInitializeComponents();

    this->movementComponent = Cast<UNyoomMovementComponent>(Super::GetMovementComponent());
}

void ANyoomCharacter::ClearJumpInput(float delta) {
    if (this->movementComponent->bCheatFlying || CVarAutoBunnyHop->GetInt() != 0) {
        return;
    }

    Super::ClearJumpInput(delta);
}

void ANyoomCharacter::OnMovementModeChanged(EMovementMode prevMode, uint8 prevCustomMode) {
    if (!bPressedJump) {
        ResetJumpState();
    }

    if (this->movementComponent->IsFalling() && bProxyIsJumpForceApplied) {
        ProxyJumpForceStartedTime = GetWorld()->GetTimeSeconds();
    } else {
        JumpCurrentCount = 0;
        JumpKeyHoldTime = 0.f;
        JumpForceTimeRemaining = 0.f;
        bWasJumping = false;
    }

    K2_OnMovementModeChanged(prevMode, this->movementComponent->MovementMode, prevCustomMode, this->movementComponent->CustomMovementMode);
    MovementModeChangedDelegate.Broadcast(this, prevMode, prevCustomMode);
}