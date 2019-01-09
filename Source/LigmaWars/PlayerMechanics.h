// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerVolume.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine//PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "PlayerMechanics.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGMAWARS_API UPlayerMechanics : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMechanics();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//The main game object
	AActor* Owner;

	//Collectible identifier
	UPROPERTY(EditAnywhere)
		AActor* Collectible;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UInputComponent * InputComponent = nullptr;
	void SetupInputComponent();

	void DestroyObjectOnOverlap();
};
