// Fill out your copyright notice in the Description page of Project Settings.

#include "EG/Public/GameFramework/EGGameModeBase.h"
#include "EngineUtils.h"
#include "EG/Public/GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "EG/Public/GameFramework/EGGameStateBase.h"
#include "GameFramework/EGPlayerState.h"

void AEGGameModeBase::BeginPlay()
{
    Super::BeginPlay();
}

void AEGGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
    
    if (GameState && GameState->PlayerArray.Num() >= 6)
    {
        ErrorMessage = TEXT("ServerError_MaxPlayersReached");
        return;
    }
}

void AEGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(NewPlayer))
    {
        APlayingPlayerControllers.Add(EGPC);
        int32 UniqueId = ++CurrentPlayerIndex;
        EGPC->SetPlayerIndex(UniqueId);
        SetRoomLeader();
            
        if (AEGGameStateBase* EGGS = GetGameState<AEGGameStateBase>())
        {
            FAward Entry;
            Entry.PlayerIndex    = UniqueId;
            Entry.PlayerEggScore = 0;
            EGGS->LeaderboardSnapshot.Add(Entry);
        }
    }
}

void AEGGameModeBase::SetRoomLeader()
{
    if (APlayingPlayerControllers.Num() > 0)
    {
        if (APlayingPlayerControllers[0].IsValid())
        {
            LeaderNum = APlayingPlayerControllers[0]->PlayerIndex;
        }
    }
}

void AEGGameModeBase::Logout(AController* Exiting)
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

void AEGGameModeBase::InitializeSpawnPoint()
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

AActor* AEGGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    InitializeSpawnPoint();
    int32 PlayerNum = GetNumPlayers()-1;
        if (AEGPlayerStart** FoundStart = PlayerStartList.Find(PlayerNum))
        {
            return *FoundStart;
        }
    EG_LOG_ROLE(LogMS, Warning, TEXT("No SpawnPoint found for index %d, using Super."), PlayerNum);
    return Super::ChoosePlayerStart_Implementation(Player);
}

void AEGGameModeBase::GameStart(int32 UniqueID)
{
    if (UniqueID == LeaderNum)
    {
        if (GetNumPlayers() > 1)
        {
            
        }
    }
}