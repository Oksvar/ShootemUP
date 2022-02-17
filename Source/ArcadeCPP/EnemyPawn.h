// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/ShootComponent.h"
#include "Components/HealthComponent.h"
#include "Actors/Bonuses/Bonus.h"
#include "EnemyPawn.generated.h"

USTRUCT(BlueprintType)
struct FBonusChance 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus")
	TSubclassOf<ABonus> BonusClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus")
	float Chance;
};

UCLASS()
class ARCADECPP_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnBonuses();

	UFUNCTION()
	void KillPawn();


	UFUNCTION()
	void OnEnemyOverlap(AActor* OverlapedActor, AActor* OtherActor);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category="Pawn")
	void DestroyPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")//������ ������� ���� ������ ue4 � ������
		UBoxComponent*PawnCollision; //������ �� ���������

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Pawn") //������ ������� ���� ������ ue4 � ������
		UStaticMeshComponent* PawnMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shooting")
		UShootComponent* ShootComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shooting")
		UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	int DestroyPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bonus")
	TArray<FBonusChance> PossibleBonuses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	UParticleSystem* DestroyParticle;
};