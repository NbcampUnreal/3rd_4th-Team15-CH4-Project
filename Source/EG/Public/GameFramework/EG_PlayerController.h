// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EG_API AEG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Player")
	int32 PlayerIndex;
    
    void SetPlayerIndex(int32 NewIndex);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
