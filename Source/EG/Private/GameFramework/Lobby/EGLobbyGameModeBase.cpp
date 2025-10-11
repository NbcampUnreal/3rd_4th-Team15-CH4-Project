// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Lobby/EGLobbyGameModeBase.h"

#include "GameFramework/EGPlayerController.h"
#include "GameFramework/EGGameInstance.h"

#include "EngineUtils.h"
#include "GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "Character/Egg/EggPoolManagerSubsystem.h"
#include "GameFramework/EGGameStateBase.h"
#include "GameFramework/EGPlayerState.h"
#include "Character/Egg/EggPoolManagerSubsystem.h"

AEGLobbyGameModeBase::AEGLobbyGameModeBase()
{
    bUseSeamlessTravel = true;
}

void AEGLobbyGameModeBase::SendChatMessage(const FString& Message)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AEGPlayerController* EGPC = Cast<AEGPlayerController>(*It);
        if (EGPC)
        {
            EGPC->ClientAddChatMessage(Message);
        }
    }
}

void AEGLobbyGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if(UEGGameInstance * GI = GetGameInstance<UEGGameInstance>())
    {
        GI->PlayerIndexReset();
    }

    // JM : 오브젝트 풀 Init Pool
    if (EggPoolDataAsset)
    {
        if (UEggPoolManagerSubsystem* PoolManager = GetWorld()->GetSubsystem<UEggPoolManagerSubsystem>())
        {
            PoolManager->InitPools(EggPoolDataAsset);
        }
    }
    else
    {
        EG_LOG_ROLE(LogJM, Warning, TEXT("EggPoolDataAsset is null"));
    }
}

void AEGLobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(NewPlayer))
    {

		// 김효영 : 처음 접속한 플레이어에게만 레벨변경 위젯 보이기
        if (!bChiefPlayer) // 처음 접속한 플레이어만
        {
            bChiefPlayer = true;

            // 레벨변경 위젯 보이기
            EGPC->ShowChiefPlayerUI();
        }

        // =================================
        if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
        {
            GI->SetPlayerIndex(1);
        }

        EGPC->ClientRPCFadeOutScreen(); // 처음 접속했을 때 페이드아웃 (작성자 : 김세훈)
    }
    if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(NewPlayer->PlayerState))
    {
        // SeamlessTravel을 대비해 서버에서만 처리
        if (HasAuthority())
        {
            if (!APlayingPlayerStates.Contains(EGPS))
            {
                APlayingPlayerStates.Add(EGPS);
            }

            UE_LOG(LogTemp, Log, TEXT("Player joined: Controller=%s | PlayerId=%d"),
                *NewPlayer->GetName(), EGPS->GetPlayerId());
        }
    }
    
    if (AEGGameStateBase* GS = GetWorld()->GetGameState<AEGGameStateBase>())
    {
        GS->UpdateLeaderboard();
    }
}

void AEGLobbyGameModeBase::Logout(AController* Exiting)
{
    if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(Exiting))
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("player %d logout."), EGPS->GetPlayerId());
        APlayingPlayerStates.RemoveAll([EGPS](const TWeakObjectPtr<AEGPlayerState>& P)
            {
                return !P.IsValid() || P.Get() == EGPS;
            });
    }
    if (bChiefPlayer && APlayingPlayerStates.Num() > 0 && !bLevelChanging)
    {
        if (AEGPlayerController* NewChief = Cast<AEGPlayerController>(APlayingPlayerStates[0]->GetOwner()))
        {
            bChiefPlayer = true;
            NewChief->ShowChiefPlayerUI();
        }
    }
    if (AEGGameStateBase* GS = GetWorld()->GetGameState<AEGGameStateBase>())
    {
        GS->UpdateLeaderboard();
    }
    
    Super::Logout(Exiting);
}

void AEGLobbyGameModeBase::InitializeSpawnPoint()
{
    PlayerStartList.Empty();

    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("[why] World is invalid. Abort building PlayerStartList."));
    }

    for (TActorIterator<AEGPlayerStart> It(World); It; ++It)
    {
        AEGPlayerStart* PS = *It;
        if (!IsValid(PS))
        {
            continue;
        }
        PlayerStartList.Add(PS->GetPlayerIndex(), PS);
    }
}

AActor* AEGLobbyGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    InitializeSpawnPoint();

    int32 PlayerNum = GetNumPlayers() - 1;

    if (AEGPlayerStart** FoundStart = PlayerStartList.Find(PlayerNum))
    {
        return *FoundStart;
    }

    EG_LOG_ROLE(LogMS, Warning, TEXT("No SpawnPoint found for index %d, using Super."), PlayerNum);
    return Super::ChoosePlayerStart_Implementation(Player);
}

// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
void AEGLobbyGameModeBase::LevelChange()
{
    FadeInScreen();

    UEGGameInstance* EGGI = Cast<UEGGameInstance>(GetGameInstance());
    if (EGGI)
    {
        EGGI->ChangeLevel();
		bLevelChanging = true;
    }
}

void AEGLobbyGameModeBase::FadeInScreen()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(It->Get()))
        {
            EGPC->ClientRPCFadeInScreen();
        }
    }
}

void AEGLobbyGameModeBase::FadeOutScreen()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(It->Get()))
        {
            EGPC->ClientRPCFadeOutScreen();
        }
    }
}

#pragma endregion
