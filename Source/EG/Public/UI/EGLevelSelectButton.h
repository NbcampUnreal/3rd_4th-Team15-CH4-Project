// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGLevelSelectButton.generated.h"

class UButton;

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
    int32 LevelNum = 1;

protected:
    UFUNCTION()
    void HandleClicked();
	
};
