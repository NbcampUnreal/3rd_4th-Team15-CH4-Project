// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/EGTitlePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AEGTitlePlayerController::BeginPlay()
{
	if (!IsLocalController())
	{
		return;
	}

	if (IsValid(UIWidgetClass))
	{
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
		if (IsValid(UIWidgetInstance))
		{
			UIWidgetInstance->AddToViewport();

			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
			SetInputMode(Mode);

			bShowMouseCursor = true;
		}
	}
}

void AEGTitlePlayerController::JoinServer(const FString& InIPAddress)
{
	FName NextLevelName = FName(*InIPAddress);
	UGameplayStatics::OpenLevel(GetWorld(), NextLevelName, true);
}
