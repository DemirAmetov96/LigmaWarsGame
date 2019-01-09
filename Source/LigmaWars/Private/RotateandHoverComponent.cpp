// Fill out your copyright notice in the Description page of Project Settings.

#include "RotateandHoverComponent.h"
#include "GameframeWork/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
URotateandHoverComponent::URotateandHoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotateandHoverComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (Owner->HasAuthority()) {
		Owner->SetReplicates(true);
		Owner->SetReplicateMovement(true);
	}
	
}

void URotateandHoverComponent::MakeObjectRotate(float DeltaTime)
{
	FRotator currentRotation = Owner->GetActorRotation();
	currentRotation.Yaw = currentRotation.Yaw + (RotationSpeed * DeltaTime);
	Owner->SetActorRotation(currentRotation);
}

void URotateandHoverComponent::MakeObjectFloat(float DeltaTime)
{
	//Get the location of the actor every frame
	CurrentLocation = Owner->GetActorLocation();

	//Delta Time is the time between each frame. If we add the DeltaTime each frame, we get the total running time (RunningTime)
	RunningTime = RunningTime + DeltaTime;

	//We want the change in height to be smooth, so we use a sin function
	SinWave = (FMath::Sin(RunningTime * FloatingSpeed));

	//Change the z position every frame. SinWave returns values between 1 and -1, but we wont notice that in the world its too small, so we multiply the change by FloatingHeight
	CurrentLocation.Z = CurrentLocation.Z + (SinWave * FloatingHeight * DeltaTime);

	//We set the new updated current location onto the actor to move it
	Owner->SetActorLocation(CurrentLocation);
}


// Called every frame
void URotateandHoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Owner->HasAuthority()) {

		//Function that makes the object float smoothly
		MakeObjectFloat(DeltaTime);

		//Function that rotates the object
		MakeObjectRotate(DeltaTime);
	}
}

