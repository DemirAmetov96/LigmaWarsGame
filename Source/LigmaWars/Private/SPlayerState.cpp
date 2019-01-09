// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"

void ASPlayerState::AddScore(float ScoreDelta)
{
	Score += ScoreDelta;
}

float ASPlayerState::GetScore() const
{
	return Score;
}

void ASPlayerState::PlayerIsReady()
{
	ReadyState = true;
}

void ASPlayerState::PlayerNotReady()
{
	ReadyState = false;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, ReadyState);
}