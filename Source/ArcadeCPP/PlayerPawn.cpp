#include "PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
APlayerPawn::APlayerPawn()
	:
	TouchMoveSensivity(2.f),
	MoveLimit(FVector2D(1100.f, 600.f))
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PawnCollision"));//Создание колизии для регистрации попаданий
	RootComponent = PawnCollision;
	PawnCollision->SetCollisionProfileName("Pawn");
	PawnCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);


	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(PawnCollision, NAME_None); //Присоединение рут компонента

	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));//Создание колизии для регистрации попаданий

	ShootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("ShootComponent"));
}
void APlayerPawn::PossessedBy(AController * NewController)
{
	PlayerController = Cast<APlayerController>(NewController);

}
bool APlayerPawn::CanBeDamaged_Implementation()
{
	return bCanBeDamaged;

}

void APlayerPawn::ExplodePawn_Implementation()
{
	ShootComponent->StopShooting();

	SetActorEnableCollision(false);

	PawnMesh->SetMaterial(0, RecoverMaterial);

	if (DestroyParticle)
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyParticle, GetActorTransform(), true);

	
	for (UActorComponent* Component : GetComponentsByClass(UParticleSystemComponent::StaticClass())) 
	{
		Component->Deactivate();
	}
}

void APlayerPawn::RecoverPawn_Implementation()
{

	SetActorEnableCollision(true);

	ShootComponent->StartShooting();

	PawnMesh->SetMaterial(0, PawnMaterial);

	for (UActorComponent* Component : GetComponentsByClass(UParticleSystemComponent::StaticClass()))
	{
		Component->Activate(true);
	}
}



// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	

	PawnMaterial = PawnMesh->GetMaterial(0);
}

float APlayerPawn::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController * Instigatedby, AActor * DamageCauser)
{
	if (!CanBeDamaged()) return 0.f;
	
	Super::TakeDamage(Damage, DamageEvent, Instigatedby, DamageCauser);
	PawnDamaged.Broadcast();
	return Damage;
}



// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindTouch(IE_Pressed, this, &APlayerPawn::OnTouchPress);
	//InputComponent->BindTouch(IE_Released, this, &APlayerPawn::OnTouchRelease);
	InputComponent->BindTouch(IE_Repeat, this, &APlayerPawn::OnTouchMove);
}

//void APlayerPawn::RevieveAnyDamage(float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
//{



//}

//Touch controls

void APlayerPawn::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	FVector2D TouchDeltaMove = FVector2D(TouchLocation.X - Location.X, TouchLocation.Y - Location.Y);

	TouchDeltaMove = TouchDeltaMove * TouchMoveSensivity;

	FVector NewLocation = GetActorLocation();
	NewLocation.X = FMath::Clamp(NewLocation.X + TouchDeltaMove.Y, -MoveLimit.Y, MoveLimit.Y);
	NewLocation.Y = FMath::Clamp(NewLocation.Y + TouchDeltaMove.X*-1.f, -MoveLimit.X, MoveLimit.X);

	SetActorLocation(NewLocation);

	TouchLocation = FVector2D(Location.X, Location.Y);
}

void APlayerPawn::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{
	TouchLocation = FVector2D(Location.X, Location.Y);
}