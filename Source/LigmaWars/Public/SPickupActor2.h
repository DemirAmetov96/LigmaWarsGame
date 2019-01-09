// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor2.generated.h"

class USphereComponent;
class UDecalComponent;
class ASPowerupActor2;

UCLASS()
class LIGMAWARS_API ASPickupActor2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickupActor2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditInstanceOnly, Category = "PickUpActor")
	TSubclassOf<ASPowerupActor2> PowerupClass;

	ASPowerupActor2* PowerupInstance;

	UPROPERTY(EditInstanceOnly, Category = "PickUpActor")
	float CoolDownDuration;

	FTimerHandle TimerHadle_RespawnTimer;

	void Respawn();

public:	

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	
};
