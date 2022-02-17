// Fill out your copyright notice in the Description page of Project Settings.

#include "BonusPoints.h"
#include "Kismet/GameplayStatics.h"
#include "ArcadeCPPGameModeBase.h"

void ABonusPoints::BonusCollected_Implementation()
{
	
	AArcadeCPPGameModeBase* Gamemode = Cast<AArcadeCPPGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (Gamemode) Gamemode->AddPoints(Points);

	Super::BonusCollected_Implementation();
}


