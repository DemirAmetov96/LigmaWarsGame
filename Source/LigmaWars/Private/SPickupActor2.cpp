// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickupActor2.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "SPowerupActor2.h"
#include "TimerManager.h"


// Sets default values
ASPickupActor2::ASPickupActor2()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;


	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);


	CoolDownDuration = 10.0f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPickupActor2::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		Respawn();
	}
}

void ASPickupActor2::Respawn()
{
	if (PowerupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Powerup us nullptr in %s ! Update Blueprint"), *GetName());
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerupInstance = GetWorld()->SpawnActor<ASPowerupActor2>(PowerupClass, GetTransform(), SpawnParams);
}

void ASPickupActor2::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//Grant a power up to player if available
	if (Role == ROLE_Authority && PowerupInstance)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			PowerupInstance->ActivatePowerup(OtherActor);

			PowerupInstance = nullptr;

			//Set Timer to respawn
			GetWorldTimerManager().SetTimer(TimerHadle_RespawnTimer, this, &ASPickupActor2::Respawn, CoolDownDuration);
		}
	}
}


