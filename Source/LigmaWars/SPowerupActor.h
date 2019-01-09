// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class LIGMAWARS_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Time between power up ticks
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float PowerupInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		float TotalNumberOfTicks;

	FTimerHandle TimerHandle_PowerupTick;

	//Total number of ticks applied
	int32 TicksProcess;

	FVector CurrentLocation;
	float SinWave, RunningTime;

	UPROPERTY(EDITANYWHERE)
		int FloatingHeight = 100;
	UPROPERTY(EDITANYWHERE)
		int FloatingSpeed = 3;
	UPROPERTY(EDITANYWHERE)
		int RotationSpeed = 50;

	void MakeObjectRotate(float DeltaTime);
	void MakeObjectFloat(float DeltaTime);

	UFUNCTION()
	void OnTickPowerup();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivatePowerup();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpired();
	
};
