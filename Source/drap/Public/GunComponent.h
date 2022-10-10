// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GunComponent.generated.h"


enum EAmmoType
{
	Raycast = 0,
	Projectile,
};

enum EFireMode
{
	Single = 0,
	Burst,
	Auto,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAP_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EAmmoType AmmoType;
	EFireMode FireMode;

	void Fire(FVector origin, FVector direction);
};
