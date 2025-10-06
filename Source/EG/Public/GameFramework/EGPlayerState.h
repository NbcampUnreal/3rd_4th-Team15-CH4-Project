// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EG_API AEGPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AEGPlayerState();
	void SetPlayerID(int32 Num);
	int32 GetPlayerID()
	{
		return CurrentPlayerID;
	}

protected:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerEggCount, BlueprintReadOnly, Category="Score")
	int32 PlayerEggCount;
	
	UPROPERTY(Replicated)
	int32 CurrentPlayerID;
	UPROPERTY(Replicated)
	FString PlayerName;
	
	UFUNCTION()
	void OnRep_PlayerEggCount();

	
	
public:
	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetPlayerEggCount() const { return PlayerEggCount; }

	UFUNCTION(Server, Reliable)
	void ServerAddEgg(int32 Amount = 1);

	UFUNCTION(Server, Reliable)
	void ServerRemoveEgg(int32 Amount = 1);
	
	void AddEgg_Internal(int32 Amount);
	void RemoveEgg_Internal(int32 Amount);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
