// Fill out your copyright notice in the Description page of Project Settings.


#include "QuakeMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UQuakeMovementComponent::UQuakeMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Friction = 10.0f;
	GroundAcceleration = 10000.0f;
	GroundMaxSpeed = 20000.0f;
	AirAcceleration = 20000.0f;
	AirMaxSpeed = 50000.0f;
	JumpImpulse = 100.0f;
}

void UQuakeMovementComponent::SetTargetDirection(
	FVector forward, FVector right, FVector input
) {
	AActor* owner = GetOwner();
	TargetDirection = forward * input.Y 
					+ right * input.X;
}

void UQuakeMovementComponent::Jump()
{
	if (!IsOnGround())
		return;

	FVector velocity = GetRootVelocity();
	velocity.Z = JumpImpulse;
	SetRootVelocity(velocity);
	// CapsuleComponent->AddImpulse(FVector(0.f, 0.f, JumpImpulse), NAME_None, true);
}

FVector UQuakeMovementComponent::GetRootVelocity() const
{
	return CapsuleComponent->GetPhysicsLinearVelocity();
}

// Called when the game starts
void UQuakeMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PreviousVelocity = GetRootVelocity();
}

// Called every frame
void UQuakeMovementComponent::TickComponent(
	float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// float zVelocity = PreviousVelocity.Z;

	bool isOnGround = IsOnGround();

	FVector velocity = isOnGround ? MoveGround(DeltaTime) : MoveAir(DeltaTime);
	velocity.Z = PreviousVelocity.Z;
	if (!isOnGround)
		velocity.Z += GetWorld()->GetGravityZ() * DeltaTime;


	SetRootVelocity(FVector(velocity.X, velocity.Y, velocity.Z));

	// FVector acceleration = velocity - PreviousVelocity;
	// SetRootVelocity(FVector(velocity.X, velocity.Y, 0.f));
}

/**
 * @brief Raycasts to check if the character is on the ground.
 * 
 * @return true Character is on the ground.
 * @return false Duh.
 */
bool UQuakeMovementComponent::IsOnGround() const
{
	FHitResult hitResult;

	FVector origin = CapsuleComponent->GetRelativeLocation() 
				   - FVector(0.0f, 0.0f, (CapsuleComponent->GetScaledCapsuleHalfHeight()));

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(CapsuleComponent->GetOwner());
	// queryParams.AddIgnoredComponent(Capsule);

	GetWorld()->LineTraceSingleByChannel(
		hitResult,
		origin + FVector(0, 0,  5.0f),
		origin + FVector(0, 0, -5.0f),
		ECollisionChannel::ECC_WorldStatic,
		queryParams
	);

	return hitResult.HitObjectHandle.IsValid();
}


FVector UQuakeMovementComponent::MoveGround(const float& deltaTime)
{
	float speed = PreviousVelocity.Length();
	if (speed != 0.0f)
	{
		float drop = speed * Friction * deltaTime;
		PreviousVelocity *= FMath::Max(speed - drop, 0.f) / speed;
	}
	
	return Accelerate(GroundAcceleration, GroundMaxSpeed, deltaTime);
}

FVector UQuakeMovementComponent::MoveAir(const float& deltaTime)
{
	return Accelerate(AirAcceleration, AirMaxSpeed, deltaTime);
}

FVector UQuakeMovementComponent::Accelerate(
	float accelerationFactor, float maxSpeed, const float& deltaTime
) {
	float projectedVelocity = PreviousVelocity.Dot(TargetDirection);
	float accelerationSpeed = deltaTime * accelerationFactor;

	if (projectedVelocity + accelerationSpeed > maxSpeed)
		accelerationSpeed = maxSpeed - projectedVelocity;
	return PreviousVelocity + TargetDirection * accelerationSpeed;
}

void UQuakeMovementComponent::SetRootVelocity(const FVector& vector)
{
	CapsuleComponent->SetPhysicsLinearVelocity(vector, false);
	PreviousVelocity = GetRootVelocity();
	// CapsuleComponent->AddForce(vector, NAME_None, true);
}


