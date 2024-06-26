// Fill out your copyright notice in the Description page of Project Settings.


#include "ACharacter.h"

// Sets default values
AACharacter::AACharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CursorSpeed   = 1.0f;
	PitchRange	  = FVector(-80.0f, 89.0f, 0.0f);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider Shape"));
	RootComponent = Capsule;
	Capsule->SetCapsuleHalfHeight(90.0f);
	Capsule->SetCapsuleRadius(45.0f);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));
	Capsule->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Capsule->SetSimulatePhysics(true);
	Capsule->BodyInstance.bLockXRotation = true;
	Capsule->BodyInstance.bLockYRotation = true;
	Capsule->BodyInstance.bLockZRotation = true;

	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));
	Body->SetCapsuleHalfHeight(90.0f);
	Body->SetCapsuleRadius(40.0f);
	Body->SetCollisionProfileName(TEXT("Pawn"));
	Body->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Body->SetSimulatePhysics(false);

	Movement = CreateDefaultSubobject<UQuakeMovementComponent>(TEXT("Movement Component"));
	Movement->RegisterComponent();
	Movement->CapsuleComponent = Capsule;

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	Health->RegisterComponent();

	Gun = CreateDefaultSubobject<UGunComponent>(TEXT("Gun Component"));
	Gun->RegisterComponent();

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(0, 0, 90.f));

	LogInfoOnScreen = true;
}

// Called when the game starts or when spawned
void AACharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AACharacter::BeginPlay()"));
	GEngine->AddOnScreenDebugMessage(100, 5.f, FColor::Red, TEXT("Playing..."));
}

// Called every frame
void AACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LogInfoOnScreen)
		LogInfo();
}

// Called to bind functionality to input
void AACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, TEXT("Player was possessed."));

	PlayerInputComponent->BindAxis<AACharacter>("Horizontal", this, &AACharacter::SetHorizontalInput);
	PlayerInputComponent->BindAxis<AACharacter>("Vertical", this, &AACharacter::SetVerticalInput);
	PlayerInputComponent->BindAxis<AACharacter>("MouseHorizontal", this, &AACharacter::SetHorizontalLookInput);
	PlayerInputComponent->BindAxis<AACharacter>("MouseVertical", this, &AACharacter::SetVerticalLookInput);
	PlayerInputComponent->BindAction<AACharacter>("Jump", IE_Pressed, this, &AACharacter::Jump);
	PlayerInputComponent->BindAction<AACharacter>("Shoot", IE_Released, this, &AACharacter::Fire);
}

void AACharacter::SetHorizontalInput(float value)
{
	m_Input.X = value;
	Movement->SetTargetDirection(
		Camera->GetForwardVector(),
		Camera->GetRightVector(),
		 m_Input
	);
}

void AACharacter::SetVerticalInput(float value)
{
	m_Input.Y = value;
	Movement->SetTargetDirection(
		Camera->GetForwardVector(),
		Camera->GetRightVector(),
		 m_Input
	);
}

void AACharacter::Jump()
{
	Movement->Jump();
}

void AACharacter::Fire()
{
	Gun->Fire(
		Capsule->GetRelativeLocation()+Camera->GetRelativeLocation(),
		Camera->GetForwardVector()
	);
}

void AACharacter::SetLookInput(USceneComponent* pSceneComponent, FVector v)
{
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::White, FString::Printf(TEXT("Mouse: (%f, %f, %f)"), v.X, v.Y, v.Z));

	FRotator rot = pSceneComponent->GetRelativeRotation();
	rot.Pitch = FMath::Clamp(rot.Pitch + v.X * CursorSpeed, PitchRange.X, PitchRange.Y);;
	rot.Yaw += v.Y * CursorSpeed; 
	
	pSceneComponent->SetRelativeRotation(rot, false, 0, ETeleportType::TeleportPhysics);
}

void AACharacter::LockCapsuleRotation() 
{
	// Capsule->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
}

void AACharacter::SetHorizontalLookInput(float value)
{
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::White, FString::Printf(TEXT("Mouse X: (%f"), value));
	m_LookInput.X = 0.0f;
	m_LookInput.Y = value;
	SetLookInput(Camera, m_LookInput);
}

void AACharacter::SetVerticalLookInput(float value)
{
	GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::White, FString::Printf(TEXT("Mouse Y: (%f"), value));
	m_LookInput.X = value;
	m_LookInput.Y = 0.0f;
	SetLookInput(Camera, m_LookInput);
}

void AACharacter::LogInfo()
{
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::White, FString::Printf(TEXT("Velocity: (%f, %f, %f)"), m_Input.X, m_Input.Y, m_Input.Z));
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::White, FString::Printf(TEXT("Is On Ground: %u"), Movement->IsOnGround()));
}
