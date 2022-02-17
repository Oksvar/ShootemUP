// Fill out your copyright notice in the Description page of Project Settings.

#include "ShootProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/DamageType.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AShootProjectile::AShootProjectile()
	:
	ProjectileSpeed(2000.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	RootComponent = Collision;
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionProfileName("NoCollision");	

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(Collision);

}

// Called when the game starts or when spawned
void AShootProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{	
		UBoxComponent* OwnerCollision = GetOwner()->FindComponentByClass<UBoxComponent>();
		Collision->IgnoreComponentWhenMoving(OwnerCollision, true);
		OwnerCollision->IgnoreComponentWhenMoving(Collision, true);

		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AShootProjectile::OnProjectileOverlap);
	
}

void AShootProjectile::OnProjectileOverlap(UPrimitiveComponent* OpelappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool Sweep, const FHitResult& Hit)
{
	APawn* OtherPawn = Cast<APawn>(OtherActor);
	if (!OtherActor || !OtherPawn) return; 

	if (!GetOwner()) return;
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner) return;
	AController* Instigator = PawnOwner->GetController();

	if (!PawnOwner->GetController() && !OtherPawn->GetController()) return;

	UGameplayStatics::ApplyDamage(OtherActor, Damage, Instigator, this, UDamageType::StaticClass());

	Destroy();
}

// Called every frame
void AShootProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector(ProjectileSpeed*DeltaTime, 0.f, 0.f));
}

