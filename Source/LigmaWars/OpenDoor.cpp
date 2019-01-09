// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameframeWork/PlayerController.h"
#include "GameFramework/Actor.h"

class ASGameState;

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (Owner->HasAuthority()) {
		Owner->SetReplicates(true);
		Owner->SetReplicateMovement(true);
	}
	MyGameState = Cast<ASGameState>(GetWorld()->GetGameState());
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Owner->HasAuthority())
	{

		if (CheckCurrentWave())
		{
			OnOpenRequest.Broadcast();
		}
	}
}

bool UOpenDoor::CheckCurrentWave()
{
	if (MyGameState)
	{
		CurrentWaveDoor = MyGameState->GetCurrentWave();
		if (CurrentWaveDoor == 7)
		{
			return true;
		}
	}
	return false;
}