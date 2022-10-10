// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

struct DamageArgs
{
	float Damage;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRAP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(DamageArgs damage);

	UPROPERTY(EditAnywhere, Category = "Health")
	int MaxHealth;

private:
	int currentHealth;
};
