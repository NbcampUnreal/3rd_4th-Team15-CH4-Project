// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFreamework/EG_GameStateBase.h"

#include "EG/EGLog.h"

#include "Net/UnrealNetwork.h"


void AEG_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AlivePlayerControllerCount);
	DOREPLIFETIME(ThisClass, MatchState);
	DOREPLIFETIME(ThisClass, RemainingCountdown);
	DOREPLIFETIME(ThisClass, RemainingPlayTime);
}

void AEG_GameStateBase::OnRep_RemainingCountdown()
{
    //UE_LOG(LogTemp, Log, TEXT("Countdown updated: %d"), RemainingCountdown);
	EG_LOG_NET(LogEGNet, Log, TEXT("Begin: %d"), RemainingCountdown);
}

void AEG_GameStateBase::OnRep_RemainingPlayTime()
{
    UE_LOG(LogTemp, Log, TEXT("Play Time: %d"), RemainingPlayTime);
    
}
