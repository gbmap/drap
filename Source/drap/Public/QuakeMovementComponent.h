// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "QuakeMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAP_API UQuakeMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuakeMovementComponent();

	UPROPERTY(EditAnywhere, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Friction;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float GroundAcceleration;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float GroundMaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AirAcceleration;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AirMaxSpeed;

	UPROPERTY(EditAnywhere, Category= "Movement")
	float JumpImpulse;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetTargetDirection(FVector input);
	void Jump();

	bool IsOnGround() const;

private:
	FVector TargetDirection;
	FVector PreviousVelocity;

	FVector GetRootVelocity() const;
	void SetRootVelocity(const FVector& velocity);

	FVector Accelerate(float accelerationFactor, float maxVelocity, const float& deltaTime);
	FVector MoveGround(const float& deltaTime);
	FVector MoveAir(const float& deltaTime);
};
