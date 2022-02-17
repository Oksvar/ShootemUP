// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcadeCPPGameModeBase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"

AArcadeCPPGameModeBase::AArcadeCPPGameModeBase():
PlayerRecoverTime(3), CurrentShootLevel(-1), IncreaseDifficultyPeriod(4.f)
{
	EnemySpawnController = CreateDefaultSubobject<UEnemySpawnController>(TEXT("EnemySpawnController"));
	HealthsComponent = CreateDefaultSubobject<UGameHealthComponent>(TEXT("HealthsComponent"));
}

void AArcadeCPPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	HealthsComponent->HealthsEnded.AddDynamic(this, &AArcadeCPPGameModeBase::Endgame);

	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (!PlayerPawn) return;

	ChangeShootLevel(true);

	PlayerPawn->PawnDamaged.AddDynamic(this, &AArcadeCPPGameModeBase::ExplodePawn);

	GetWorld()->GetTimerManager().SetTimer(IncreaseDifficultyTimer, this, &AArcadeCPPGameModeBase::IncreaseDifficulty, IncreaseDifficultyPeriod, true);

}

void AArcadeCPPGameModeBase::ExplodePawn_Implementation()
{
	PlayerPawn->ExplodePawn();
	HealthsComponent->Changehealths(-1);

	ChangeShootLevel(false);

	if(!IsGameOver)
	GetWorld()->GetTimerManager().SetTimer(RecoverTimer, this, &AArcadeCPPGameModeBase::RecoverPawn, PlayerRecoverTime, false);
}

void AArcadeCPPGameModeBase::RecoverPawn_Implementation()
{

	PlayerPawn->RecoverPawn();

}

void AArcadeCPPGameModeBase::Endgame()
{
	IsGameOver = true;

	EnemySpawnController->SetActive(false);

	GameOver.Broadcast();

	UGameplayStatics::GetPlayerPawn(this, 0)->Destroy();

	UE_LOG(LogTemp, Log, TEXT("Game OVER"));

	SetPause(UGameplayStatics::GetPlayerController(this, 0), false);
}

void AArcadeCPPGameModeBase::IncreaseDifficulty()
{

	EnemySpawnController->ChangeStageTimeMultiplier = FMath::Max(EnemySpawnController->ChangeStageTimeMultiplier * 0.95, 0.4);
}

void AArcadeCPPGameModeBase::AddPoints(int Points)
{
	GamePoints += Points;

}

bool AArcadeCPPGameModeBase::ChangeShootLevel(bool Up)
{
	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!PlayerPawn) return false;

	int NewLevel = FMath::Clamp(CurrentShootLevel + (Up ? 1 : -1), 0, ShootInfoLevels.Num() - 1);

	if (NewLevel == CurrentShootLevel) return false;

	CurrentShootLevel = NewLevel;

	PlayerPawn->ShootComponent->ShootInfos = ShootInfoLevels[CurrentShootLevel].ShootInfos;
	PlayerPawn->ShootComponent->ShootPeriod = ShootInfoLevels[CurrentShootLevel].ShootPeriod;

	return true;
}