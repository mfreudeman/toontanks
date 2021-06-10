// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

class APawnTurret;
class APawnTank;
class APlayerControllerBase;

UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void ActorDied(AActor* DeadActor);

	void PlayerHealthDidChange(float CurrentHealth, float MaxHealth);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent)
	void Respawn();
	
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bPlayerWon);

	UFUNCTION(BlueprintImplementableEvent)
	void TurretCountChanged(int32 TurretsRemaining);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerHealthUpdated(float CurrentHealth, float MaxHealth);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	int32 StartDelay = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	int32 NumberOfLives = 3;

private:

	APawnTank* PlayerTank;

	APlayerControllerBase* PlayerControllerRef;
	
	int32 GetTargetTurretCount();
	int32 NumberOfTargetTurrets = 0;

	void HandleGameStart();
	void HandleRespawn();
	void HandleGameOver(bool bPlayerWon);
};
