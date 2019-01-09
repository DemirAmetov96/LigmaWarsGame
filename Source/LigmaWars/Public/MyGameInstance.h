// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LIGMAWARS_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
		UFUNCTION(BlueprintCallable, Category = "Networking", meta = (DisplayName = "My Server Travel"))
		void MyServerTravel(FString mapName, FString gameMode, bool bAbsolute, FString additionalOptions);

		UPROPERTY(BlueprintReadWrite)
			int32 PlayersReady;

		UFUNCTION(BlueprintCallable)
			int32 GetPlayersReady();
};
