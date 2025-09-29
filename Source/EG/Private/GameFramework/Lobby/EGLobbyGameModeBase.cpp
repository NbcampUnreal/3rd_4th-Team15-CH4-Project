// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Lobby/EGLobbyGameModeBase.h"

#include "GameFramework/EGPlayerController.h"
#include "GameFramework/EGGameInstance.h"

#include "EngineUtils.h"
#include "GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "GameFramework/EGGameStateBase.h"
#include "GameFramework/EGPlayerState.h"
#include "AI/EGAICharacter.h"
#include "GameFramework/EGInGameSpawnPoints.h"


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

}

void AEGLobbyGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
    FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    if (APlayingPlayerControllers.Num() >= 6)
    {
        ErrorMessage = TEXT("ServerError_MaxPlayersReached");
        return;
    }
}


void AEGLobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // maxplayer check
    if (APlayingPlayerControllers.Num() > 6)
    {
        UE_LOG(LogTemp, Warning, TEXT("Too many players! Kick that chick"));

        NewPlayer->Destroy();
    }

    if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(NewPlayer))
    {
        APlayingPlayerControllers.Add(EGPC);

        if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(EGPC->PlayerState))
        {
            int32 UniqueId = EGPS->GetPlayerId();
            EGPC->SetPlayerIndex(UniqueId);

            if (AEGGameStateBase* EGGS = GetGameState<AEGGameStateBase>())
            {
                FAward Entry;
                Entry.PlayerIndex = UniqueId;
                Entry.PlayerEggScore = 0;
                EGGS->LeaderboardSnapshot.Add(Entry);
            }
        }
    }

    SetRoomLeader();
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

// 굳이 필요한가? 어차피 레벨 이동은 버튼을 통해서 이동하는데. 버튼을 방장한테만 보이게 하면 되지 않을까? 위에 코드는 로그인과 로그아웃할 때마다 방장 설정하는 코드이니까
void AEGLobbyGameModeBase::GameStart(int32 UniqueID)
{
    if (UniqueID == LeaderNum)
    {
        if (GetNumPlayers() > 1)
        {
            if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
            {
                GI->ChangeLevel();
            }
            
        }
    }
}