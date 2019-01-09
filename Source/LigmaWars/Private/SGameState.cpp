// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameState.h"
#include "Net/UnrealNetwork.h"

class ASGameMode;

ASGameState::ASGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentWave = 0;
	CountDownTimer = 10.0f;
	CountDownIsVisible = true;
}

void ASGameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

// Called when the game starts or when spawned
void ASGameState::BeginPlay()
{
	Super::BeginPlay();
	
	MyGameMode = (ASGameMode*)GetWorld()->GetAuthGameMode();
	CurrentGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (CurrentGameInstance)
	{
		PlayersReadyFromLobby = CurrentGameInstance->GetPlayersReady();
	}
}

// Called every frame
void ASGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MyGameMode)
	{
		if (MyGameMode->GetAllCurrentPlayers() == PlayersReadyFromLobby)
		{
			if (CountDownTimer > 0)
			{
				CountDownTimer = CountDownTimer - DeltaTime;
				CountDownTimerInteger = (int32)CountDownTimer;
				UE_LOG(LogClass, Log, TEXT("The current countdown timer is: %f"), CountDownTimer);
			}
			else
			{
				UE_LOG(LogClass, Log, TEXT("Now changing the countdown timer visible to false"));
				CountDownIsVisible = false;
				PrimaryActorTick.bCanEverTick = false;
			}
		}
	}	
}

int32 ASGameState::GetCurrentWave()
{
	return CurrentWave;
}

void ASGameState::SetWaveState(EWaveState NewState)
{
	if (Role == ROLE_Authority)
	{
		EWaveState OldState = WaveState;

		WaveState = NewState;
		//Call on server
		OnRep_WaveState(OldState);
		if (WaveState == EWaveState::WaveComplete)
		{
			CurrentWave++;
		}
	}
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, WaveState);
	DOREPLIFETIME(ASGameState, CountDownTimer);
	DOREPLIFETIME(ASGameState, CurrentWave);
	DOREPLIFETIME(ASGameState, CountDownIsVisible);
}