// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/UW_TitleLayout.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/EGTitlePlayerController.h"

UUW_TitleLayout::UUW_TitleLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_TitleLayout::NativeConstruct()
{
	PlayButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnPlayButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void UUW_TitleLayout::OnPlayButtonClicked()
{
	AEGTitlePlayerController* PlayerController = GetOwningPlayer<AEGTitlePlayerController>();
	if (IsValid(PlayerController))
	{
		//FText ServerIP = ServerIPEditableText->GetText();
		FString ServerIP = "43.203.30.199:7777";
		//PlayerController->JoinServer(ServerIP.ToString());
		PlayerController->JoinServer(ServerIP);
	}
}

void UUW_TitleLayout::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}