// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/EGPlayerState.h"
#include "GameFramework/EGGameStateBase.h"
#include "Net/UnrealNetwork.h"


AEGPlayerState::AEGPlayerState()
{
	CurrentPlayerID = -1;
	PlayerName = FString::Printf(TEXT("Player %d"), CurrentPlayerID);
	PlayerEggCount = 0;
}

void AEGPlayerState::SetPlayerID(int32 Num)
{
	CurrentPlayerID = Num;
	PlayerName = FString::Printf(TEXT("Player %d state On"), CurrentPlayerID);
}

void AEGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEGPlayerState, PlayerEggCount);
	DOREPLIFETIME(AEGPlayerState, CurrentPlayerID);
}

void AEGPlayerState::OnRep_PlayerEggCount()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerEggCount -> %s replicated egg count: %d"),
		*GetName(), PlayerEggCount); 
}

void AEGPlayerState::ServerAddEgg_Implementation(int32 Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerAddEgg_Implementation called on %s with Amount: %d"),
		*GetName(), Amount);

	AddEgg_Internal(Amount);
}

void AEGPlayerState::ServerRemoveEgg_Implementation(int32 Amount)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerRemoveEgg_Implementation called on %s with Amount: %d"),
		*GetName(), Amount);

	RemoveEgg_Internal(Amount);
}

void AEGPlayerState::AddEgg_Internal(int32 Amount)
{
	PlayerEggCount = FMath::Max(0, PlayerEggCount + Amount);

	UE_LOG(LogTemp, Warning, TEXT("AddEgg_Internal -> %s new egg count: %d"),
		*GetName(), PlayerEggCount);
	if (AEGGameStateBase* GS = GetWorld()->GetGameState<AEGGameStateBase>())
	{
		GS->UpdateLeaderboard();
	}
}

void AEGPlayerState::RemoveEgg_Internal(int32 Amount)
{
	PlayerEggCount = FMath::Max(0, PlayerEggCount - Amount);

	UE_LOG(LogTemp, Warning, TEXT("RemoveEgg_Internal -> %s new egg count: %d"),
		*GetName(), PlayerEggCount);
		if (AEGGameStateBase* GS = GetWorld()->GetGameState<AEGGameStateBase>())
		{
			GS->UpdateLeaderboard();
		}
}

void AEGGameStateBase::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	UpdateLeaderboard();
}