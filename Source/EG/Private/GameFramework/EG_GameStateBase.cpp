// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFreamework/EG_GameStateBase.h"

#include "EG/EGLog.h"

#include "Net/UnrealNetwork.h"


void AEG_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	EG_LOG_NET(LogJM, Log, TEXT(""));

	DOREPLIFETIME(ThisClass, AlivePlayerControllerCount);
	DOREPLIFETIME(ThisClass, MatchState);
	DOREPLIFETIME(ThisClass, RemainingCountdown);
	DOREPLIFETIME(ThisClass, RemainingPlayTime);
}

void AEG_GameStateBase::OnRep_RemainingCountdown()
{
	EG_LOG_NET(LogJM, Log, TEXT("Countdown updated: %d"), RemainingCountdown);

	OnCountdownUpdated.Broadcast(RemainingCountdown);
}

void AEG_GameStateBase::OnRep_RemainingPlayTime()
{
    UE_LOG(LogTemp, Log, TEXT("Play Time: %d"), RemainingPlayTime);

	OnPlayTimeUpdated.Broadcast(RemainingPlayTime);    
}
