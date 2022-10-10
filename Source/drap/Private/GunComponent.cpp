// Fill out your copyright notice in the Description page of Project Settings.


#include "GunComponent.h"
#include "GameFramework/Actor.h"

#include "../Public/HealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunComponent::Fire(
	FVector origin, 
	FVector direction
) {
	FHitResult hitResult;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());

	// queryParams.AddIgnoredComponent(Capsule);
	
	GetWorld()->LineTraceSingleByChannel(
		hitResult,
		origin,
		origin + direction * 10000.0f,
		ECollisionChannel::ECC_WorldStatic,
		queryParams
	);

	DrawDebugLine(
		GetWorld(),
		origin,
		origin + direction * 10000.0f,
		FColor::Red,
		false,
		1.0f,
		0,
		1.0f
	);

	if (hitResult.GetActor() == nullptr)
		return;

	if (hitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *hitResult.GetActor()->GetName());
	}

	auto hitObject = hitResult.GetActor();
	UHealthComponent* health = hitObject->FindComponentByClass<UHealthComponent>();
	if (health != nullptr) 
	{
		DamageArgs damage;
		damage.Damage = 1.0f;
		health->TakeDamage(damage);
	}

	// hitObject->AddImpulseAtLocation(direction * 100000.0f, hitResult.ImpactPoint);
	UPrimitiveComponent* primitive = hitObject->FindComponentByClass<UPrimitiveComponent>();
	if (primitive != nullptr)
	{
		primitive->AddImpulseAtLocation(
			direction * 10000.0f,
			hitResult.ImpactPoint
		);
	}

	UStaticMeshComponent* mesh = hitObject->FindComponentByClass<UStaticMeshComponent>();
	if (mesh != nullptr)
	{
		mesh->AddImpulseAtLocation(
			direction * 20000.0f,
			hitResult.ImpactPoint
		);
	}
}

