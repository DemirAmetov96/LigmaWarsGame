// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"
#include "SHealthComponent.h"
#include "SGameState.h"
#include "SPlayerState.h"
#include "TimerManager.h"

ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 10.0f;

	GameStateClass = ASGameState::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}



void ASGameMode::StartWave()
{
	for (FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; it++)
	{
		ASCharacter *PlayerCharacter = Cast<ASCharacter>(it->Get());
		if (PlayerCharacter)
		{
			if (PlayerCharacter->WaveFinishSound)
			{
				PlayerCharacter->PlayWaveEndSound();
			}
			if (PlayerCharacter->WaveInProgressAmbiance)
			{
				PlayerCharacter->PlayWaveInProgressAmbiance();
			}
		}
	}

	WaveCount++;

	NrOfBotsToSpawn = 2 * WaveCount;

	UE_LOG(LogClass, Log, TEXT("Just set the number of bots to spawn to = %d"), NrOfBotsToSpawn);

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplte);

}

void ASGameMode::PrepareForNextWave()
{
	if (AllCurrentPlayers == PlayersReadyFromLobby)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);

		SetWaveState(EWaveState::WaitingToStart);

		TimeBetweenWaves = 2.0f;

		RestartDeadPlayers();
	}
}

void ASGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NrOfBotsToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();

		if (TestPawn == nullptr || TestPawn->IsPlayerControlled() || TestPawn->ActorHasTag("BossAI"))
		{
			continue;
		}

		USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		if (AllCurrentPlayers == PlayersReadyFromLobby)
		{
			SetWaveState(EWaveState::WaveComplete);

			PrepareForNextWave();
		}
	}
}

void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				//A player is still alive
				return;
			}
			else
			{
				//No Player Alive
				GameOver();
			}
		}
	}
}

void ASGameMode::GameOver()
{
	EndWave();

	//Finish The match, present game over to players
	SetWaveState(EWaveState::GameOver);

	UE_LOG(LogTemp, Log, TEXT("GAME OVER LOSER! PLAYERS ALL FREAKIN DEAD"))
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GS = GetGameState<ASGameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(NewState);
	}
}

void ASGameMode::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();
	CurrentGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (CurrentGameInstance)
	{
		PlayersReadyFromLobby = CurrentGameInstance->GetPlayersReady();
	}
	
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();

	CheckAnyPlayerAlive();
}

void ASGameMode::GenericPlayerInitialization(AController* Controller)
{
	AllCurrentPlayers = AllCurrentPlayers + 1;
}

int32 ASGameMode::GetAllCurrentPlayers()
{
	return AllCurrentPlayers;
}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NrOfBotsToSpawn--;
	UE_LOG(LogClass, Log, TEXT("Just subtracted from the number of bots to spawn, it now = %d"), NrOfBotsToSpawn);

	if (NrOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}