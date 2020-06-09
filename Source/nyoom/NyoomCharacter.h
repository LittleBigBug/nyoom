// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "NyoomMovementComponent.h"
#include "NyoomCharacter.generated.h"

UCLASS()
class NYOOM_API ANyoomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANyoomCharacter(const FObjectInitializer& objectInitializer);

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Mapped input functions
	void InputStrafe(float val);
	void InputForward(float val);

	// Crouching inputs
	void InputCrouch();
	void InputUnCrouch();

	// Custom Movement component
    UNyoomMovementComponent* movementComponent;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	void PostInitializeComponents() override;

	void ClearJumpInput(float delta) override;
	void OnMovementModeChanged(EMovementMode prevMode, uint8 prevCustomMode) override;

    // Allow BP to access movement components
    UFUNCTION(BlueprintCallable, Category = "Movement")
    FORCEINLINE class UNyoomMovementComponent* GetNyoomMovementComponent() const {
        return this->movementComponent;
    }

    // Returns a rounded velocity to 2 places and converts to desired unit
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetReadableVelocity();

};
