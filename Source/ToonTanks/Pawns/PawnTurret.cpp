// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"

APawnTurret::APawnTurret()
{
}

void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn || GetDistanceToPlayer() > FireRange)
	{
		return;
	}

	RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::RotateTurret(FVector LookAtTarget)
{
	Super::RotateTurret(LookAtTarget);
}

void APawnTurret::FireProjectile()
{
	Super::FireProjectile();
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
}

void APawnTurret::CheckFireCondition()
{
	// If player == null || player is dead
	//		Then Return
	// 
	// If player is in range
	//		Then Fire!

	if (!PlayerPawn)
	{
		return;
	}

	if (GetDistanceToPlayer() <= FireRange)
	{
		FireProjectile();
	}
}

float APawnTurret::GetDistanceToPlayer()
{
	if (PlayerPawn)
	{
		return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());;
	}
	else
	{
		return 0.f;
	}
}