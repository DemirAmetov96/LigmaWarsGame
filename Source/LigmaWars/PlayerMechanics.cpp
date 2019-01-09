// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMechanics.h"
#include "GameframeWork/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPlayerMechanics::UPlayerMechanics()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlayerMechanics::BeginPlay()
{
	Super::BeginPlay();
	SetupInputComponent();
	Owner = GetOwner();
}

void UPlayerMechanics::DestroyObjectOnOverlap() {
	UE_LOG(LogTemp, Warning, TEXT("Running the DestroyObjectOnOverlap function"));
	if (Owner->IsOverlappingActor(Collectible))
	{
		UE_LOG(LogTemp, Warning, TEXT("The owner is overlapping the collectible"));
		Collectible->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroyed the collectible object"));
	}
}

void UPlayerMechanics::SetupInputComponent()
{
	//Look for the attached Input component that appears on runtime.
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found the input component"));
		InputComponent->BindAction("Pickup", IE_Pressed, this, &UPlayerMechanics::DestroyObjectOnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s mmissing input component component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UPlayerMechanics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}