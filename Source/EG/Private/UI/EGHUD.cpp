// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EGHUD.h"

#include "UI/EGChatting.h"
#include "UI/EGChatMessage.h"
#include "Components/ScrollBox.h"


void AEGHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController)
	{
		if (ChattingClass)
		{
			Chatting = CreateWidget<UEGChatting>(PlayerController, ChattingClass);
			AddChatting();
		}
	}
}

void AEGHUD::ChiefPlayerSetting()
{
	if (ChiefPlayerClass)
	{
		APlayerController* PlayerController = GetOwningPlayerController();
		if (PlayerController)
		{
			UUserWidget* ChiefPlayerWidget = CreateWidget<UUserWidget>(PlayerController, ChiefPlayerClass);
			if (ChiefPlayerWidget)
			{
				ChiefPlayerWidget->AddToViewport(1);
			}
		}
	}
}

void AEGHUD::AddChatting()
{
	if (Chatting)
	{
		Chatting->AddToViewport(2);
	}
}

void AEGHUD::AddChatMessage(const FString& Message)
{
	APlayerController* OwningPlayer = Cast<APlayerController>(GetOwningPlayerController());
	Chatting = Chatting == nullptr ? CreateWidget<UEGChatting>(OwningPlayer, ChattingClass) : Chatting;

	if (OwningPlayer && ChattingClass && Chatting && ChatMessageClass)
	{
		// �޽��� �� �ٿ� �ش��ϴ� ������ ����
		UEGChatMessage* ChatMessageWidget = CreateWidget<UEGChatMessage>(OwningPlayer, ChatMessageClass);
		if (ChatMessageWidget)
		{
			// Scroll box�� AddChild ����
			ChatMessageWidget->SetChatMessage(Message);
			Chatting->ChatScrollBox->AddChild(ChatMessageWidget);
			Chatting->ChatScrollBox->ScrollToEnd();
			Chatting->ChatScrollBox->bAnimateWheelScrolling = true;			
		}
	}
}
