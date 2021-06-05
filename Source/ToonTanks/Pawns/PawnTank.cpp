// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Rotate();
	Move();

	if (PlayerControllerRef)
	{
		FHitResult TraceHitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;

		RotateTurret(HitLocation);
	}
}

void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::FireProjectile);
}

void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::RotateTurret(FVector LookAtTarget)
{
	Super::RotateTurret(LookAtTarget);
}

void APawnTank::FireProjectile()
{
	Super::FireProjectile();
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
	
	bPlayerIsAlive = false;

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

bool APawnTank::IsPlayerAlive()
{
	return bPlayerIsAlive;
}

void APawnTank::CalculateMoveInput(float value)
{
	MoveDirection = FVector(value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0.f, 0.f);
}

void APawnTank::CalculateRotateInput(float value)
{
	float RotateAmount = value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0.f, RotateAmount, 0.f);
	RotationDirection = FQuat(Rotation);
}

void APawnTank::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}
