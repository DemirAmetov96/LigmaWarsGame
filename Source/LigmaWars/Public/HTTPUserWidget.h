// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "HTTPUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LIGMAWARS_API UHTTPUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// expose this function so that it can be called via a node from BP
	UFUNCTION(BlueprintCallable)
		void PlayButtonClicked();

	FHttpModule* Http;

	//Assign this function to call when the GET request processes successfully
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//Sets default values for this actor's properties
	UHTTPUserWidget(const class FObjectInitializer& ObjectInitializer);

};
