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
	// Sets default values for this actor's properties
	AEG_PlayerStart();
	int GetPlayerIndex();
	
	UPROPERTY(VisibleAnywhere, EditAnywhere)
	int PlayerIndex;	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
