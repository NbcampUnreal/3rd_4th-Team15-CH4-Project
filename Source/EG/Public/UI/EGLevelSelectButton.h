// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGLevelSelectButton.generated.h"

class UButton;
class ULevelImageDataAsset;


/**
 * 
 */
UCLASS()
class EG_API UEGLevelSelectButton : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* Button;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    FString MapName = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    bool bRightMove = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TObjectPtr<ULevelImageDataAsset> LevelImageData;

protected:
    UFUNCTION()
    void HandleClicked();
	
    UFUNCTION()
    void UpdateMapImage();

};
