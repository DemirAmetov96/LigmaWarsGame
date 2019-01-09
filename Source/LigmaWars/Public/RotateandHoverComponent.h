// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotateandHoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGMAWARS_API URotateandHoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotateandHoverComponent();

protected:
	// Called when the game starts
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* Owner = nullptr;

};
