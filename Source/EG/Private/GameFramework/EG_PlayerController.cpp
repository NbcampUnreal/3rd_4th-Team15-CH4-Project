// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/EG_PlayerController.h"
#include "EGLog.h"
#include "Net/UnrealNetwork.h"


void AEG_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEG_PlayerController, PlayerIndex);
}

void AEG_PlayerController::SetPlayerIndex(int32 NewIndex)
{
	if (HasAuthority())
	{
		PlayerIndex = NewIndex;
		EG_LOG_ROLE(LogMS, Warning, TEXT("Player %d is online."), PlayerIndex);
	}
}