// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;
	GameModeRef = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);

	CheckAndNotifyPlayerHealthChange(GetOwner());
}

void UHealthComponent::TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage == 0 || Health <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);

	UE_LOG(LogTemp, Warning, TEXT("%s Hit! New health: %f"), *GetOwner()->GetName(), Health);

	CheckAndNotifyPlayerHealthChange(DamageActor);

	if (Health <= 0)
	{
		if (GameModeRef)
		{
			GameModeRef->ActorDied(GetOwner());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Health Component has no reference to the Game Mode"));
		}
	}
}

void UHealthComponent::CheckAndNotifyPlayerHealthChange(AActor* DamageActor)
{
	if (APawnTank* PlayerPawn = Cast<APawnTank>(DamageActor))
	{
		if (GameModeRef)
		{
			GameModeRef->PlayerHealthDidChange(Health, DefaultHealth);
		}
	}
}


