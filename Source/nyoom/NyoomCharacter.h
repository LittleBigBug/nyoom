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

	// Calculates new velocity
	void CalcMovement();

	// Calculates applied friction on velocity
	void CalcFriction();

	// Mapped input functions
	void InputStrafe(float val);
	void InputForward(float val);

	// Player input vector
	// x = forward/backward
	// y = strafe
	FVector2D playerInput;

	// Player Velocity
	FVector velocity;

	// Custom Movement component
    UNyoomMovementComponent* movementComponent;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	void PostInitializeComponents() override;

    // Allow BP to access movement components
    UFUNCTION(BlueprintCallable, Category = "Movement")
    FORCEINLINE class UNyoomMovementComponent* GetNyoomMovementComponent() const {
        return this->movementComponent;
    }

	// Allow access to velocity
    UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetMovementVel() {
	    return this->velocity;
	}

};
