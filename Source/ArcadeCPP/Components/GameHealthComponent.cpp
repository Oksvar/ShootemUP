// Fill out your copyright notice in the Description page of Project Settings.

#include "GameHealthComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGameHealthComponent::UGameHealthComponent():
	Healths(3)
{
	
}


// Called when the game starts
void UGameHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("No playerpawn"));
		return;
	}

	
}


void UGameHealthComponent::Changehealths(int ByValue)
{
	Healths += ByValue;

	HealthsChanged.Broadcast(ByValue);

	if (Healths <= 0)
	{
		HealthsEnded.Broadcast();
	}

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %i"), Healths);

}

int UGameHealthComponent::GetHealth()
{
	return Healths;
}


