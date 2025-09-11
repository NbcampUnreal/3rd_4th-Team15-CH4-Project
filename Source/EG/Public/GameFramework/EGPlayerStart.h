// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "EGPlayerStart.generated.h"

UCLASS()
class EG_API AEGPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	AEGPlayerStart(const FObjectInitializer& ObjectInitializer);	
	int32 GetPlayerIndex();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerIndex")
	int32 PlayerIndex;	

};
