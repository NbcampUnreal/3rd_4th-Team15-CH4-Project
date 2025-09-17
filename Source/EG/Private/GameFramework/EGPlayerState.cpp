// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/EGPlayerState.h"
#include "Net/UnrealNetwork.h"

void AEGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEGPlayerState, PlayerEggCount);
}

AEGPlayerState::AEGPlayerState()
{
}

void AEGPlayerState::OnRep_PlayerEggCount()
{
	if (HasAuthority())
	{
		// ui update	
	}
}

void AEGPlayerState::ServerAddEgg_Implementation(int32 Amount)
{
	AddEgg_Internal(Amount);
}

void AEGPlayerState::ServerRemoveEgg_Implementation(int32 Amount)
{
	RemoveEgg_Internal(Amount);
}

void AEGPlayerState::AddEgg_Internal(int32 Amount)
{
	PlayerEggCount = FMath::Max(0, PlayerEggCount + Amount);
}

void AEGPlayerState::RemoveEgg_Internal(int32 Amount)
{
	PlayerEggCount = FMath::Max(0, PlayerEggCount - Amount);
}