// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/EGPlayerController.h"
#include "EGLog.h"
#include "Net/UnrealNetwork.h"

// ===== 내가 추가한 include =====
#include "UI/WBP_HUD.h"               // ★ BP 부모 C++ 위젯
#include "AbilitySystemComponent.h"    // ASC / FOnAttributeChangeData
#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
// =================================

// 김효영
#include "GameFramework/EGGameModeBase.h"
#include "UI/EGHUD.h"
#include "UI/EGChatting.h" 

void AEGPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEGPlayerController, PlayerIndex);
}

void AEGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController())
	{
		return;
	}

	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);

	// ===== 내가 추가한 부분 =====
	CreateAndShowHUD();
	BindHUDToASC();
	if (!HUDRef || !FindASC())
	{
		GetWorldTimerManager().SetTimer(InitAscTimer, this, &AEGPlayerController::TryInitHUD_ASC, 0.2f, true);
	}
	// ===== 여기까지 =====
}

void AEGPlayerController::SetPlayerIndex(int32 NewIndex)
{
	if (HasAuthority())
	{
		PlayerIndex = NewIndex;
		EG_LOG_ROLE(LogMS, Warning, TEXT("Player %d is online."), PlayerIndex);
	}
}


// ====== 내가 추가한 구현 ======

void AEGPlayerController::CreateAndShowHUD()
{
	if (!HUDRef && HUDWidgetClass)
	{
		HUDRef = CreateWidget<UWBP_HUD>(this, HUDWidgetClass);
		if (HUDRef)
		{
			HUDRef->AddToViewport();
		}
	}
}

UAbilitySystemComponent* AEGPlayerController::FindASC() const
{
	if (APawn* P = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = P->FindComponentByClass<UAbilitySystemComponent>())
		{
			return ASC;
		}
	}
	if (APlayerState* PS = GetPlayerState<APlayerState>())
	{
		if (UAbilitySystemComponent* ASC = PS->FindComponentByClass<UAbilitySystemComponent>())
		{
			return ASC;
		}
	}
	return nullptr;
}

void AEGPlayerController::BindHUDToASC()
{
	if (!HUDRef) return;

	if (UAbilitySystemComponent* ASC = FindASC())
	{
		HUDRef->InitWithASC(ASC);
		if (InitAscTimer.IsValid())
		{
			GetWorldTimerManager().ClearTimer(InitAscTimer);
		}
	}
}

void AEGPlayerController::TryInitHUD_ASC()
{
	BindHUDToASC();
}

// 한국인


// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
void AEGPlayerController::ServerRequestLevelChange_Implementation(const FString& MapName)
{
	if (HasAuthority())
	{
		AEGGameModeBase* GM = Cast<AEGGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->ChangeLevel(MapName);
		}
	}
}

#pragma endregion

// 채팅 (작성자 : 김효영)
#pragma region Chatting
void AEGPlayerController::ActivateChatBox()
{
	EGHUD = EGHUD == nullptr ? Cast<AEGHUD>(GetHUD()) : EGHUD;

	if (EGHUD && EGHUD->Chatting)
	{
		EGHUD->Chatting->ActivateChatText();
	}
}

void AEGPlayerController::ClientAddChatMessage_Implementation(const FString& Message)
{
	EGHUD = EGHUD == nullptr ? Cast<AEGHUD>(GetHUD()) : EGHUD;
	if (EGHUD)
	{
		EGHUD->AddChatMessage(Message);
	}
}

void AEGPlayerController::ServerSendChatMessage_Implementation(const FString& Message)
{
	AEGGameModeBase* GM = GetWorld()->GetAuthGameMode<AEGGameModeBase>();
	if (GM)
	{
		GM->SendChatMessage(Message);
	}	
}

#pragma endregion