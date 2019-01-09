// Fill out your copyright notice in the Description page of Project Settings.

#include "HTTPUserWidget.h"

// Sets default values
UHTTPUserWidget::UHTTPUserWidget(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get();
}

/*Http call*/
void UHTTPUserWidget::PlayButtonClicked()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPUserWidget::OnResponseReceived);
	//This is the url on which to process the request
	Request->SetURL("http://35.204.188.179:28020/joinlobby");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

//Assigned function on successful HTTP call
void UHTTPUserWidget::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name
		FString destinationAddress = JsonObject->GetStringField("Destination");

		//Output it to the console
		UE_LOG(LogTemp, Warning, TEXT("The destination is: %s"), *destinationAddress);

		GetWorld()->GetFirstPlayerController()->ClientTravel(*destinationAddress, TRAVEL_Absolute);

		UWorld* TheWorld = GetWorld();

		FString CurrentLevel = TheWorld->GetMapName();

		if (CurrentLevel == "MainMenu")
		{
			//UGameplayStatics::Client(GetWorld(), FName(*destinationAddress));
		}
	}
}