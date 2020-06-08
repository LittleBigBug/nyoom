// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NyoomMovementComponent.generated.h"

UCLASS()
class NYOOM_API UNyoomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

    UNyoomMovementComponent();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GroundAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AirAcceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AirSpeedCap;

public:
    void CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration) override;
    void ApplyVelocityBraking(float delta, float friction, float brakingDeceleration) override;

    bool DoJump(bool bClientSimulation) override;

    void TwoWallAdjust(FVector& delta, const FHitResult& hit, const FVector& oldHitNormal) const override;
    FVector HandleSlopeBoosting(const FVector& slideResult, const FVector& delta, const float time, const FVector& normal, const FHitResult& hit) const override;
    bool ShouldCatchAir(const FFindFloorResult& oldFloor, const FFindFloorResult& newFloor) override;

    float SlideAlongSurface(const FVector& delta, float time, const FVector& normal, FHitResult& hit, bool bHandleImpact = false);

    float GetMaxSpeed() const override;

private:
    void PreemptCollision(float delta);

    bool bLastGrounded = false;
};
