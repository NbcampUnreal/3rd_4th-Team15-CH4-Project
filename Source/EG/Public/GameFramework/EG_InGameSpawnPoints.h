// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EG_InGameSpawnPoints.generated.h"

UCLASS()
class EG_API AEG_InGameSpawnPoints : public AActor
{
	GENERATED_BODY()

public:
	AEG_InGameSpawnPoints();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnPoint")
	int32 SpawnSortNumber;
};