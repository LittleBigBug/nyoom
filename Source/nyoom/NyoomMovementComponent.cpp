// Fill out your copyright notice in the Description page of Project Settings.

#include "NyoomMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UNyoomMovementComponent::UNyoomMovementComponent() {
    AirControl = 1.f;

    AirControlBoostMultiplier = 1.f;
    AirControlBoostVelocityThreshold = 0.f;

    // 450 Hammer units -> 857.25 unreal units
    MaxAcceleration = 857.25f;

    MaxWalkSpeed = 361.9f;

    WalkSpeed = 285.75f;

    GroundAcceleration = 10.f;
    AirAcceleration = 10000.f;

    AirSpeedCap = 57.15f;

    GroundFriction = 4.f;
    BrakingFriction = 4.f;
    bUseSeparateBrakingFriction = false;

    BrakingFrictionFactor = 1.f;

    FallingLateralFriction = 0.f;

    BrakingDecelerationFalling = 0.f; // No air friction
    BrakingDecelerationFlying = 190.5f;
    BrakingDecelerationSwimming = 190.5f;
    BrakingDecelerationWalking = 190.5f;

    MaxStepHeight = 34.29f;

    JumpZVelocity = 304.8f;
    JumpOffJumpZFactor = 1.f;

    bCanWalkOffLedgesWhenCrouching = true;

    // 45 degrees
    SetWalkableFloorZ(0.7f);

    StandingDownwardForceScale = 1.f;

    InitialPushForceFactor = 100.f;
    PushForceFactor = 500.f;

    RepulsionForce = 0.f;
    MaxTouchForce = 0.f;
    TouchForceFactor = 0.f;

    bPushForceUsingZOffset = false;
    PushForcePointZOffsetFactor = -0.66f;

    bScalePushForceToVelocity = true;

    bPushForceScaledToMass = false;
    bTouchForceScaledToMass = false;

    Mass = 65.f;

    bUseControllerDesiredRotation = false;
    bUseFlatBaseForFloorChecks = true;
}

void UNyoomMovementComponent::CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration) {
    if (!HasValidData() || HasAnimRootMotion() || delta < MIN_TICK_TIME || (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)) {
        return;
    }

    friction = FMath::Max(0.f, friction);

    const float maxAccel = GetMaxAcceleration();
    float maxSpeed = GetMaxSpeed();

    bool bZeroRequestedAccel = true;
    float requestedSpeed = 0.f;
    FVector requestedAccel = FVector::ZeroVector;

    bool reqMove = ApplyRequestedMove(delta, maxAccel, maxSpeed, friction, brakingDeceleration, requestedAccel, requestedSpeed);
    if (reqMove) {
        requestedAccel = requestedAccel.GetClampedToMaxSize(maxAccel);
    }

    if (bForceMaxAccel) {
        if (Acceleration.SizeSquared() > SMALL_NUMBER) {
            Acceleration = Acceleration.GetSafeNormal() * maxAccel;
        } else {
            Acceleration = maxAccel * (Velocity.SizeSquared() < SMALL_NUMBER ? UpdatedComponent->GetForwardVector() : Velocity.GetSafeNormal());
        }

        AnalogInputModifier = 1.f;
    }

    const float maxInputSpeed = FMath::Max(maxSpeed * AnalogInputModifier, GetMinAnalogSpeed());
    maxSpeed = FMath::Max(requestedSpeed, maxInputSpeed);

    const bool bZeroAccel = Acceleration.IsNearlyZero();
    const bool bIsGrounded = IsMovingOnGround();
    const bool bIsGroundMove = bIsGrounded && bLastGrounded;

    // Apply friction only if they are grounded
    if (bIsGroundMove) {
        const float actualBrakingFriction = bUseSeparateBrakingFriction ? BrakingFriction : friction;
        ApplyVelocityBraking(delta, actualBrakingFriction, brakingDeceleration);
    }

    if (bFluid) {
        Velocity *= 1.f - FMath::Min(friction * delta, 1.f);
    }

    if (bCheatFlying) {
        if (bZeroAccel) {
            Velocity = FVector::ZeroVector;
            return;
        }

        FVector lookVec = CharacterOwner->GetControlRotation().Vector();
        FVector lookVec2D = CharacterOwner->GetActorForwardVector();

        lookVec2D.Z = 0.f;

        FVector perpendicularAccel = (lookVec2D | Acceleration) * lookVec2D;
        FVector tangentialAccel = Acceleration - perpendicularAccel;
        FVector unitAccel = Acceleration;

        float lookAng = unitAccel.CosineAngle2D(lookVec);
        FVector product = lookAng * lookVec * perpendicularAccel.Size2D() + tangentialAccel;

        Velocity = product.GetClampedToSize(MaxAcceleration, MaxAcceleration);
        return;
    }

    if (!bZeroAccel) {
        Acceleration = Acceleration.GetClampedToMaxSize2D(maxSpeed);

        const FVector accelDir = Acceleration.GetSafeNormal2D();

        // Dot product is done here
        // Strafe movementtttt
        const float veer = Velocity.X * accelDir.X + Velocity.Y * accelDir.Y;
        const float addSpeed = (bIsGroundMove ? Acceleration : Acceleration.GetClampedToMaxSize2D(AirSpeedCap)).Size2D() - veer;

        if (addSpeed > 0.f) {
            float accelMult = bIsGroundMove ? GroundAcceleration : AirAcceleration;

            Acceleration *= accelMult * delta;
            Acceleration = Acceleration.GetClampedToMaxSize2D(addSpeed);

            Velocity += Acceleration;
        }
    }

    if (!bZeroRequestedAccel) {
        Velocity += requestedAccel * delta;
    }

    // Surfing
    PreemptCollision(delta);

    // Cap
    Velocity = Velocity.GetClampedToMaxSize2D(13470.4f);

    bLastGrounded = bIsGrounded;
}

bool UNyoomMovementComponent::DoJump(bool bClientSimulation) {
    return bCheatFlying || Super::DoJump(bClientSimulation);
}

void UNyoomMovementComponent::TwoWallAdjust(FVector &delta, const FHitResult &hit, const FVector &oldHitNormal) const {
    Super::Super::TwoWallAdjust(delta, hit, oldHitNormal);
}

float UNyoomMovementComponent::SlideAlongSurface(const FVector &delta, float time, const FVector &normal, FHitResult &hit, bool bHandleImpact) {
    return Super::Super::SlideAlongSurface(delta, time, normal, hit, bHandleImpact);
}

FVector UNyoomMovementComponent::HandleSlopeBoosting(const FVector &slideResult, const FVector &delta, const float time, const FVector &normal, const FHitResult &hit) const {
    return slideResult;
}

bool UNyoomMovementComponent::ShouldCatchAir(const FFindFloorResult &oldFloor, const FFindFloorResult &newFloor) {
    float speed = Velocity.Size2D();
    float maxSpeed = WalkSpeed * 1.5f;

    float speedMult = maxSpeed / speed;
    float nrmDiff = newFloor.HitResult.ImpactNormal.Z - oldFloor.HitResult.ImpactNormal.Z;

    // Catch air if the difference between the old floor normals & new floor normals is something
    // and the player's speed is low
    return (nrmDiff > 0.f && speedMult < 0.5f) || Super::ShouldCatchAir(oldFloor, newFloor);
}

void UNyoomMovementComponent::ApplyVelocityBraking(float delta, float friction, float brakingDeceleration) {
    float speed = Velocity.Size2D();

    if (speed <= 0.1f || !HasValidData() || HasAnimRootMotion() || delta < MIN_TICK_TIME) {
        return;
    }

    friction *= BrakingFrictionFactor;
    brakingDeceleration = FMath::Max(brakingDeceleration, speed);

    if (FMath::IsNearlyZero(friction) || FMath::IsNearlyZero(brakingDeceleration)) {
        return;
    }

    const FVector oldVel = Velocity;
    const FVector revAccel = friction * brakingDeceleration * oldVel.GetSafeNormal();

    Velocity -= revAccel * delta;

    if ((Velocity | oldVel) <= 0.f || FMath::IsNearlyZero(Velocity.SizeSquared(), KINDA_SMALL_NUMBER)) {
        Velocity = FVector::ZeroVector;
    }
}

void UNyoomMovementComponent::PreemptCollision(float delta) {
    if (Velocity.IsNearlyZero() || IsMovingOnGround()) {
        return;
    }

    float floorSweepTraceDist = MaxStepHeight + MAX_FLOOR_DIST + KINDA_SMALL_NUMBER;

    UCapsuleComponent* capsuleComponent = GetCharacterOwner()->GetCapsuleComponent();

    FHitResult hitResult;

    FVector offset = FVector(0.f, 0.f, capsuleComponent->GetScaledCapsuleHalfHeight());
    FVector start = capsuleComponent->GetComponentLocation() - offset;
    FVector end = start - FVector(0.f, 0.f, floorSweepTraceDist);

    GetWorld()->LineTraceSingleByChannel(hitResult, start, end, UpdatedComponent->GetCollisionObjectType(),
            FCollisionQueryParams(FName(TEXT("SurfTrace")), true, GetCharacterOwner()));

    if (!hitResult.bBlockingHit || hitResult.ImpactNormal.Z >= GetWalkableFloorZ()) {
        return;
    }

    FVector movementVec = Velocity + (capsuleComponent->GetPhysicsLinearVelocity().Z + GetGravityZ() * delta * (1.f - hitResult.ImpactNormal.Z));
    FVector collisionVec = hitResult.ImpactNormal;

    collisionVec *= -1 * (movementVec | hitResult.ImpactNormal);

    collisionVec.X = 0.f;
    collisionVec.Y = 0.f;

    float speed = Velocity.SizeSquared2D();

    // Using Squared is more efficient, Sqrt is expensive (only when we have to)
    if ((collisionVec.Z * collisionVec.Z) > speed) {
        collisionVec.Z = FMath::Sqrt(speed);
    }

    AddImpulse(collisionVec, true);
}

float UNyoomMovementComponent::GetMaxSpeed() const {
    if ((IsWalking() || IsFalling() || bCheatFlying) && IsCrouching()) {
        return WalkSpeed;
    }

    return Super::GetMaxSpeed();
}