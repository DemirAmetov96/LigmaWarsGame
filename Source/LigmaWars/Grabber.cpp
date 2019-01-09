// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
		if (GetOwner()->HasAuthority()) {
			FindPhysicsHandleComponent();
			SetupInputComponent();
			GetOwner()->SetReplicates(true);
			GetOwner()->SetReplicateMovement(true);
		}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwner()->HasAuthority())
	{
		if (!PhysicsHandle)
		{
			UE_LOG(LogTemp, Error, TEXT("Physics Handle not found"));
			return;
		}

		//if the physics handle is attached
		if (PhysicsHandle->GrabbedComponent)
		{
			//Move the object that we are holding
			PhysicsHandle->SetTargetLocation(GetReachLineEnd());
		}
	}
}

void UGrabber::FindPhysicsHandleComponent() {
	//Look for the attached Physics Handle
	if (GetOwner()->HasAuthority())
	{
		PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
		if (PhysicsHandle)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found the physics handler"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s mmissing physics handle component"), *GetOwner()->GetName());
		}
	}
}

void UGrabber::SetupInputComponent()
{
	if (GetOwner()->HasAuthority())
	{
		//Look for the attached Input component that appears on runtime.
		InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
		if (InputComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found the input component"));
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
			InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s mmissing input component component"), *GetOwner()->GetName());
		}
	}
}

void UGrabber::Grab() {
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));

		//Line trace and see if we reach any actors with physics body collision channel set
		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();
		auto ActorHit = HitResult.GetActor();

		//If we hit something then attach a physics handle
		if (ActorHit)
		{
			PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
		}
	}
}

void UGrabber::Release() {
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Grab key released"));

		//Release physics handle
		PhysicsHandle->ReleaseComponent();
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult HitResult;
	if (GetOwner()->HasAuthority())
	{
		//Line-trace (AKA ray-cast) out to reach distance

		//Setup query parameters
		FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
		GetWorld()->LineTraceSingleByObjectType(
			OUT HitResult,
			GetReachLineStart(),
			GetReachLineEnd(),
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParameters);

		AActor* ActorHit = HitResult.GetActor();
		if (ActorHit) {
			UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
		}
	}
	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	if (GetOwner()->HasAuthority())
	{
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	}
	return (PlayerViewPointLocation);
}

FVector UGrabber::GetReachLineEnd()
{
	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	if (GetOwner()->HasAuthority())
	{
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
		//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
		//Draw a red trace in the world to visualise
		//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);
	}
	return (PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach);
}