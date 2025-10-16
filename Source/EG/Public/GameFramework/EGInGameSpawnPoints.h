// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EGInGameSpawnPoints.generated.h"

UCLASS()
class EG_API AEGInGameSpawnPoints : public AActor
{
	GENERATED_BODY()

public:
	AEGInGameSpawnPoints();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnPoint")
	int32 SpawnSortNum;

public:
	int32 GetSpawnSortNum();
};