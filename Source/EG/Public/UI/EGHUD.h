// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EGHUD.generated.h"


/**
 * 
 */
UCLASS()
class EG_API AEGHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChattingClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChatMessageClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChiefPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GetOutClass;

	UPROPERTY()
	class UEGChatting* Chatting;

	virtual void PostInitializeComponents() override;

	void ChiefPlayerSetting();
	void AddChatting();
	void AddChatMessage(const FString& Message);
	void GetOut();
};
