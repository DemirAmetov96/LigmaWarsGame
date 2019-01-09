// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "SHealthComponent.h"
#include "Enemey.generated.h"

class USHealthComponent;

UCLASS()
class LIGMAWARS_API AEnemey : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemey();

protected:

	UFUNCTION()
		void SeePlayer(APawn *pawn);

	TArray<AActor*> Waypoints;
	AAIController* AIController;
	TScriptDelegate<FWeakObjectPtr> MovementCompleteDelegate;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* SensingComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ATargetPoint* GetRandomWaypoint();

	UFUNCTION()
		void AIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		USHealthComponent* HealthComp;

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Returns current start of reach line
	FVector GetReachLineStart();

	//Returns current end of reach line
	FVector GetReachLineEnd();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
