// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "EG_PlayerStart.generated.h"

UCLASS()
class EG_API AEG_PlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	AEG_PlayerStart(const FObjectInitializer& ObjectInitializer);	
	int32 GetPlayerIndex();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerIndex")
	int32 PlayerIndex;	

};
