// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateAbstractDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void APawnBase::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetClean = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();
	
	FRotator TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();
	
	TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::FireProjectile()
{
	if (ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

void APawnBase::HandleDestruction()
{
	SpawnDeathParticle();
	PlayDeathSound();
	DoDeathCameraShake();
}

void APawnBase::DoDeathCameraShake()
{
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShake);
}

void APawnBase::PlayDeathSound()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}

void APawnBase::SpawnDeathParticle()
{
	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	}
}