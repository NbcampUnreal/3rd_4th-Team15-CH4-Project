// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelImageDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLevelImage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level|Button")
	TSoftObjectPtr<UTexture2D> NormalImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level|Button")
	TSoftObjectPtr<UTexture2D> HoveredImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level|Button")
	TSoftObjectPtr<UTexture2D> PressedImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level|ButtonColor")
	FLinearColor NormalColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level|ButtonColor")
	FLinearColor HoveredColor = FLinearColor(0.385417f, 0.385417f, 0.385417f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level|ButtonColor")
	FLinearColor PressedColor = FLinearColor(0.088542f, 0.088542f, 0.088542f, 1.f);
};

UCLASS(BlueprintType)
class EG_API ULevelImageDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FLevelImage> LevelImage;
	
};
