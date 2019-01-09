// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemey.h"


// Sets default values
AEnemey::AEnemey()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Sensing Component");
	SensingComponent->OnSeePawn.AddDynamic(this, &AEnemey::SeePlayer);
	SensingComponent->SetSensingUpdatesEnabled(true);
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
}

ATargetPoint* AEnemey::GetRandomWaypoint()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling GetRandomWayPoint function"));
	int max = Waypoints.Num() - 1;
	int32 index = FMath::RandRange(0, max);
	return Cast<ATargetPoint>(Waypoints[index]);
}

void AEnemey::SeePlayer(APawn * pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Saw the character"));
	AIController->MoveToActor(pawn);
}

// Called when the game starts or when spawned
void AEnemey::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Waypoints);
	AIController = Cast<AAIController>(GetController());
	UE_LOG(LogTemp, Warning, TEXT("Begin play is running"));
	this->bUseControllerRotationYaw = false;
	if ((Waypoints.Num() > 0) && (AIController))
	{
		UE_LOG(LogTemp, Warning, TEXT("Waypoints is more than 0 and its AIcontroller"));
		MovementCompleteDelegate.BindUFunction(this, "AIMoveCompleted");
		AIController->ReceiveMoveCompleted.Add(MovementCompleteDelegate);
		AIController->MoveToActor(GetRandomWaypoint());
	}
}

void AEnemey::AIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("Running AIMoveCompleted function"));
	if (Result == EPathFollowingResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveCompleted funtion: Result = Success"));
		AIController->MoveToActor(GetRandomWaypoint());
	}
}
// Called every frame
void AEnemey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemey::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}