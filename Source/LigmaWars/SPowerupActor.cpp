// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0;

}

void ASPowerupActor::MakeObjectRotate(float DeltaTime)
{
	FRotator currentRotation = GetActorRotation();
	currentRotation.Yaw = currentRotation.Yaw + (RotationSpeed * DeltaTime);
	SetActorRotation(currentRotation);
}

void ASPowerupActor::MakeObjectFloat(float DeltaTime)
{
	//Get the location of the actor every frame
	CurrentLocation = GetActorLocation();

	//Delta Time is the time between each frame. If we add the DeltaTime each frame, we get the total running time (RunningTime)
	RunningTime = RunningTime + DeltaTime;

	//We want the change in height to be smooth, so we use a sin function
	SinWave = (FMath::Sin(RunningTime * FloatingSpeed));

	//Change the z position every frame. SinWave returns values between 1 and -1, but we wont notice that in the world its too small, so we multiply the change by FloatingHeight
	CurrentLocation.Z = CurrentLocation.Z + (SinWave * FloatingHeight * DeltaTime);

	//We set the new updated current location onto the actor to move it
	SetActorLocation(CurrentLocation);
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void ASPowerupActor::OnTickPowerup()
{
	if (HasAuthority())
	{
		TicksProcess++;

		if (TicksProcess >= TotalNumberOfTicks)
		{
			OnExpired();
			GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
		}
	}
}

// Called every frame
void ASPowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()) {

		//Function that makes the object float smoothly
		MakeObjectFloat(DeltaTime);

		//Function that rotates the object
		MakeObjectRotate(DeltaTime);
	}
}

void ASPowerupActor::ActivatePowerup()
{
	if (HasAuthority())
	{
		OnActivated();

		if (PowerupInterval > 0.0f)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true);
		}
		else
		{
			OnTickPowerup();
		}
	}
}

