// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "QuakeMovementComponent.h" 

#include "ACharacter.generated.h"


UCLASS()
class DRAP_API AACharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AACharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Components")
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, Category="Components")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Components")
	UQuakeMovementComponent* Movement;

	// =============== MOVEMENT

	UPROPERTY(EditAnywhere, Category="Movement")
	float CursorSpeed;

	UPROPERTY(EditAnyWhere, Category="Movement")
	FVector PitchRange;

	UPROPERTY(EditAnywhere, Category="Debug")
	bool LogInfoOnScreen;

private:
	FVector m_Input;
	FVector m_LookInput;

	FVector m_Velocity;
	FVector m_PreviousVelocity;


	void SetHorizontalInput(float value);
	void SetVerticalInput(float value);
	void SetVerticalLookInput(float value);
	void SetHorizontalLookInput(float value);

	void SetLookInput(USceneComponent* pSceneComponent, FVector value);

	void Jump();

	// DEBUG STUFF
	void LogInfo();
};
