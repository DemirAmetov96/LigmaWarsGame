// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SCharacter.h"
#include "SGameMode.generated.h"


enum class EWaveState : uint8;
class USoundCue;
class UMyGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

/**
 * 
 */
UCLASS()
class LIGMAWARS_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	//Bots to spawn in current wave
	int32 NrOfBotsToSpawn;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

protected:

	//Hook for BP to spawn a single bot
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();
	
	//Start spawning bots
	void StartWave();

	//Stop Spawning Bots
	void EndWave();

	//Set Timer for next wave
	void PrepareForNextWave();

	//Check if all elements from that specific wave are still alive
	void CheckWaveState();

	//Check if any player is alive to see if can make game over
	void CheckAnyPlayerAlive();
	
	void GameOver();

	void SetWaveState(EWaveState NewState);

	void RestartDeadPlayers();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void ResetHealth();

public:

	ASGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilled OnActorKilled;

	UPROPERTY(BlueprintReadWrite, Category = "GameMode")
		int32 WaveCount;

	void GenericPlayerInitialization(AController* Controller) override;

	UPROPERTY(BlueprintReadWrite)
		int32 AllCurrentPlayers;

	int32 GetAllCurrentPlayers();

	UMyGameInstance* CurrentGameInstance;

	int32 PlayersReadyFromLobby;
};