// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"


// Sets default values for this component's properties
UShootComponent::UShootComponent()
	:
	ShootPeriod(1.f)
{
	
	// ...
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();

	StartShooting();
	
}

void UShootComponent::Shoot()
{
	

	for (FShootInfo ShootInfo : ShootInfos)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(ShootInfo.Offeset);


		FRotator SpawnRotation = GetOwner()->GetActorRotation();
		SpawnRotation.Add(0.f, ShootInfo.Angle, 0.f);

		AShootProjectile* Projectule = GetWorld()->SpawnActor<AShootProjectile>(ShootInfo.ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
		if (Projectule)Projectule->Damage = ShootInfo.Damage;
	}
}

void UShootComponent::StartShooting()
{
	GetWorld()->GetTimerManager().SetTimer(ShootingTimer, this, &UShootComponent::Shoot, ShootPeriod, true, ShootPeriod);
}

void UShootComponent::StopShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootingTimer);
}

