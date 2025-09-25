// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGChatMessage.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGChatMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatMessageTextBlock;

	void SetChatMessage(const FString& Message);
};
