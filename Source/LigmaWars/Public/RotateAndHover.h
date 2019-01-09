// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateAndHover.generated.h"

UCLASS()
class LIGMAWARS_API ARotateAndHover : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateAndHover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector CurrentLocation;
	float SinWave, RunningTime;

	UPROPERTY(EDITANYWHERE)
		int FloatingHeight;
	UPROPERTY(EDITANYWHERE)
		int FloatingSpeed;
	UPROPERTY(EDITANYWHERE)
		int RotationSpeed;


	void MakeObjectRotate(float DeltaTime);
	void MakeObjectFloat(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
