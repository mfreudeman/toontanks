// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"

void ATankGameModeBase::PlayerHealthDidChange(float CurrentHealth, float MaxHealth)
{
	PlayerHealthUpdated(CurrentHealth, MaxHealth);
}

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	if (DeadActor == PlayerTank)
	{
		PlayerTank->HandleDestruction();

		if (--NumberOfLives <= 0)
		{
			HandleGameOver(false);
		}
		else
		{
			HandleRespawn();
		}
		
		if (PlayerControllerRef)
		{
			PlayerControllerRef->SetPlayerEnableState(false);
		}
	}
	else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
	{
		DestroyedTurret->HandleDestruction();
		if (--NumberOfTargetTurrets == 0)
		{
			HandleGameOver(true);
		}
		TurretCountChanged(NumberOfTargetTurrets);
	}
}

int32 ATankGameModeBase::GetTargetTurretCount()
{
	TArray<AActor*> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
	return TurretActors.Num();
}

void ATankGameModeBase::HandleGameStart()
{
	NumberOfTargetTurrets = GetTargetTurretCount();

	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	// Call Blueprint Events GameStart event creates UI class blueprints.
	GameStart();
	TurretCountChanged(NumberOfTargetTurrets);

	if (PlayerControllerRef)
	{
		PlayerControllerRef->SetPlayerEnableState(false);
		FTimerHandle PlayerEnableHandle;
		FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(
			PlayerControllerRef, &APlayerControllerBase::SetPlayerEnableState, true
		);
		GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
	}
}

void ATankGameModeBase::HandleRespawn()
{
	
}

void ATankGameModeBase::HandleGameOver(bool bPlayerWon)
{
	GameOver(bPlayerWon);
}
