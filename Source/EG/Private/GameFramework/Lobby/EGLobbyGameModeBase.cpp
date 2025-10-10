// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Lobby/EGLobbyGameModeBase.h"

#include "GameFramework/EGPlayerController.h"
#include "GameFramework/EGGameInstance.h"

#include "EngineUtils.h"
#include "GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "GameFramework/EGGameStateBase.h"
#include "GameFramework/EGPlayerState.h"


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
        EGPC->ClientHideBlackScreen();
    }
    
    
    if (AEGGameStateBase* GS = GetWorld()->GetGameState<AEGGameStateBase>())
    {
        GS->UpdateLeaderboard();
    }
}

void AEGLobbyGameModeBase::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

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
    ShowScreen();

    UEGGameInstance* EGGI = Cast<UEGGameInstance>(GetGameInstance());
    if (EGGI)
    {
        EGGI->ChangeLevel();
		bLevelChanging = true;
    }
}

void AEGLobbyGameModeBase::ShowScreen()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(It->Get()))
        {
            EGPC->ClientShowBlackScreen();
        }
    }
}

//void AEGLobbyGameModeBase::HideScreen()
//{
//    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
//    {
//        if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(It->Get()))
//        {
//            EGPC->ClientHideBlackScreen();
//        }
//    }
//}

#pragma endregion
