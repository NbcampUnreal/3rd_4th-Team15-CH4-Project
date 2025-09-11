// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/EGPlayerController.h"
#include "EGLog.h"
#include "Net/UnrealNetwork.h"


void AEGPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEGPlayerController, PlayerIndex);
}

void AEGPlayerController::SetPlayerIndex(int32 NewIndex)
{
	if (HasAuthority())
	{
		PlayerIndex = NewIndex;
		EG_LOG_ROLE(LogMS, Warning, TEXT("Player %d is online."), PlayerIndex);
	}
}