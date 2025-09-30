// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Lobby/EGLobbyGameModeBase.h"

#include "GameFramework/EGPlayerController.h"
#include "GameFramework/EGGameInstance.h"

#include "EngineUtils.h"
#include "GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "GameFramework/EGGameStateBase.h"
#include "GameFramework/EGPlayerState.h"



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
}

void AEGLobbyGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    if (GameState && GameState->PlayerArray.Num() >= 6)
    {
        ErrorMessage = TEXT("ServerError_MaxPlayersReached");
        return;
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

        APlayingPlayerControllers.Add(EGPC);
        int32 UniqueId = ++CurrentPlayerIndex;
        EGPC->SetPlayerIndex(UniqueId);
        SetRoomLeader();

        if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
        {
            GI->SetPlayerIndex(1);
        }
    }
}

void AEGLobbyGameModeBase::SetRoomLeader()
{
    if (APlayingPlayerControllers.Num() > 0)
    {
        if (APlayingPlayerControllers[0].IsValid())
        {
            LeaderNum = APlayingPlayerControllers[0]->PlayerIndex;
        }
    }
}


void AEGLobbyGameModeBase::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(Exiting))
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("player %d logout."), EGPC->PlayerIndex);
        APlayingPlayerControllers.RemoveAll([EGPC](const TWeakObjectPtr<AEGPlayerController>& P)
            {
                return !P.IsValid() || P.Get() == EGPC;
            });
    }
    SetRoomLeader();
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

//void AEGLobbyGameModeBase::GameStart(int32 UniqueID)
//{
//    if (UniqueID == LeaderNum)
//    {
//        if (GetNumPlayers() > 1)
//        {
//            if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
//            {
//                GI->ChangeLevel();
//            }
//            
//        }
//    }
//}