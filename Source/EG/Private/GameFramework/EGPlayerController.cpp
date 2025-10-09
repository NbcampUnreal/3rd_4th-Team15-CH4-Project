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
#include "GameFramework/Lobby/EGLobbyGameModeBase.h"
#include "GameFramework/EGGameInstance.h"
#include "UI/EGHUD.h"
#include "UI/EGChatting.h" 
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sounds/SFXManagerSubsystem.h"


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

/*
void AEGPlayerController::SetPlayerIndex(int32 NewIndex)
{
	if (HasAuthority())
	{
		if (HasAuthority()) // 서버에서만 PlayerIndex를 설정해야 합니다.
		{
			PlayerIndex = NewIndex;
			EG_LOG_ROLE(LogMS, Warning, TEXT("Player Index set to: %d"), PlayerIndex);    
		}
	}

}

*/


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

void AEGPlayerController::ServerRequestLevelRecordChange_Implementation(const FString& MapName)
{
	if (HasAuthority())
	{
		if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
		{
			GI->RecordLevel(MapName);
		}
	}
}

void AEGPlayerController::ServerRequestLevelChange_Implementation()
{
	if (HasAuthority())
	{
		if (AEGLobbyGameModeBase* GM = GetWorld()->GetAuthGameMode<AEGLobbyGameModeBase>())
		{
			GM->LevelChange();
		}
		/*if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
		{
			GI->ChangeLevel();
		}*/
	}
}

void AEGPlayerController::ToggleMouseCursor()
{
	bMouseVisible = !bMouseVisible;

	if (bMouseVisible)
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI()); // 게임+UI 모두 입력
	}
	else
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());  // 다시 게임 전용 입력
	}
}

void AEGPlayerController::ClientShowBlackScreen_Implementation()
{
	if (UEGGameInstance* GI = Cast<UEGGameInstance>(GetGameInstance()))
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			PC->SetIgnoreMoveInput(true);
			PC->SetIgnoreLookInput(true);
			bEnableClickEvents = false;
			bEnableMouseOverEvents = false;
		}
		GI->FadeInScreen();
	}
}

void AEGPlayerController::ClientHideBlackScreen_Implementation()
{
	if (UEGGameInstance* GI = Cast<UEGGameInstance>(GetGameInstance()))
	{
		GI->FadeOutScreen();
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			PC->SetIgnoreMoveInput(false);
			PC->SetIgnoreLookInput(false);
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;
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

void AEGPlayerController::ClientGetOutWidget_Implementation()
{
	EGHUD = EGHUD == nullptr ? Cast<AEGHUD>(GetHUD()) : EGHUD;
	if (EGHUD)
	{
		EGHUD->GetOut();
	}
}

void AEGPlayerController::ServerSendChatMessage_Implementation(const FString& Message)
{
	if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
	{
		GI->SendChatMessage(Message);
	}

	/*AEGGameModeBase* GM = GetWorld()->GetAuthGameMode<AEGGameModeBase>();
	if (GM)
	{
		GM->SendChatMessage(Message);
	}	*/
}

#pragma endregion

// 방장 기능(작성자: 김효영)
#pragma region ChiefPlayer 
void AEGPlayerController::ShowChiefPlayerUI_Implementation()
{
	EGHUD = EGHUD == nullptr ? Cast<AEGHUD>(GetHUD()) : EGHUD;

	if (EGHUD)
	{
		EGHUD->ChiefPlayerSetting();
	}
}

#pragma endregion

// JM : 게임 시작/종료시 sfx 재생
void AEGPlayerController::ClientRPC_PlaySFXGameStart_Implementation()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (USFXManagerSubsystem* SFXManager = GI->GetSubsystem<USFXManagerSubsystem>())
		{
			SFXManager->PlaySFXLocalClientOnly(ESFXType::GameStart, this);
		}
		else
		{
			EG_LOG(LogJM, Warning, TEXT("No SFXManager Subsystem"));
		}
	}
	else
	{
		EG_LOG(LogJM, Warning, TEXT("No Game Instance"));
	}
}

void AEGPlayerController::ClientRPC_PlaySFXGameOver_Implementation()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (USFXManagerSubsystem* SFXManager = GI->GetSubsystem<USFXManagerSubsystem>())
		{
			SFXManager->PlaySFXLocalClientOnly(ESFXType::GameOver, this);
		}
		else
		{
			EG_LOG(LogJM, Warning, TEXT("No SFXManager Subsystem"));
		}
	}
	else
	{
		EG_LOG(LogJM, Warning, TEXT("No Game Instance"));
	}
}

void AEGPlayerController::WinnderLogic()
{
	UE_LOG(LogTemp, Warning, TEXT("Winnder Logic"));
}

void AEGPlayerController::LoserLogic()
{
	UE_LOG(LogTemp, Warning, TEXT("Loser Logic"));
}