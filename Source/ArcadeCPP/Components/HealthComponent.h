// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEndedEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADECPP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float Health;

	UFUNCTION()
	void OnOwnerDamaged(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);

public:	
	
	UFUNCTION(BlueprintCallable, Category = "Healths")
	void ChangeHealth(float Value);

	UFUNCTION(BlueprintPure, Category = "Healths")
	float Gehealth();

	UPROPERTY(BlueprintAssignable, Category = "Healths")
	FHealthEndedEvent OnHealthEnded;
};
