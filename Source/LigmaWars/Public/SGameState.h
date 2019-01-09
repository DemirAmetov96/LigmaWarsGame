// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameMode.h"
#include "SGameState.generated.h"

class UMyGameInstance;

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,

	WaveInProgress,

	//No longer spawning new bots, waiting for players to kill remaining bots
	WaitingToComplte,

	WaveComplete,

	GameOver,
};

/**
 * 
 */
UCLASS()
class LIGMAWARS_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
		EWaveState WaveState;
	
public:
	ASGameState();

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "GameState")
		int32 CurrentWave;

	void SetWaveState(EWaveState NewState);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "GameState")
	float CountDownTimer;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "GameState")
		int32 CountDownTimerInteger;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "GameState")
		bool CountDownIsVisible;

	ASGameMode* MyGameMode;

	UMyGameInstance* CurrentGameInstance;

	int32 PlayersReadyFromLobby;

	int32 GetCurrentWave();
};