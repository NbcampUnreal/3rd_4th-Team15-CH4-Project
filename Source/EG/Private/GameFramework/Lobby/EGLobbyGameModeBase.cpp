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
#include "GameFramework/GameSession.h"

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

    FadeOutScreen(); // 처음 접속했을 때 페이드아웃 (작성자 : 김세훈)

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

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(It->Get()))
        {
            if (EGPC->bChiefPlayera == false)
            {
                EGPC->ShowChiefPlayerUI();
                EG_LOG_ROLE(LogMS, Warning, TEXT("NewChiefasdf"));
                break;
            }
        }
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
            EGPC->bChiefPlayera = true;
            // 레벨변경 위젯 보이기
            EGPC->ShowChiefPlayerUI();
        }

        // =================================
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

void AEGLobbyGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
    Super::HandleSeamlessTravelPlayer(C);

    
    if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(C))
    {
        // if (!EGPC->PlayerState)
        // {
        //     EGPC->InitPlayerState();
        // }
        // TWeakObjectPtr<AEGPlayerController> WeakPC = EGPC;
        // FTimerHandle TimerHandle;
        // GetWorldTimerManager().SetTimer(TimerHandle, [this, WeakPC]()
        // {
        // if (!WeakPC.IsValid() || !GameSession) return;
        // AEGPlayerController* PC = WeakPC.Get();
        //
        // if (PC->PlayerState && PC->PlayerState->GetUniqueId().IsValid())
        // {
        //     GameSession->RegisterPlayer(
        //         PC,
        //         PC->PlayerState->GetUniqueId().GetUniqueNetId(),
        //         false
        //     );
        //     UE_LOG(LogTemp, Log, TEXT("✅ Player re-registered: %s"), *PC->GetName());
        // }
        // else
        // {
        //     UE_LOG(LogTemp, Warning, TEXT("⚠️ Failed to register %s (Invalid UniqueId)"), *PC->GetName());
        // }
        // }, 0.2f, false);
        //
        if (!bChiefPlayer) // 처음 접속한 플레이어만
        {
            bChiefPlayer = true;
            EGPC->bChiefPlayera = true;
            // 레벨변경 위젯 보이기
            EGPC->ShowChiefPlayerUI();
        }
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

    if (AEGPlayerController* PC = Cast<AEGPlayerController>(Exiting))
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("casting is right"));

        if (PC->bChiefPlayera == true && APlayingPlayerStates.Num() > 0)
        {
            for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
            {
                if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(It->Get()))
                {
                    if (EGPC!=PC)
                    {
                        if (EGPC->bChiefPlayera == false)
                        {
                            EGPC->bChiefPlayera = true;
                            EGPC->ShowChiefPlayerUI();
                            bChiefPlayer = true;
                            EG_LOG_ROLE(LogMS, Warning, TEXT("NewChiefasdf"));
                            break;
                        }
                    }
                    else
                    {
                        EG_LOG_ROLE(LogMS, Warning, TEXT("Chief'controller is exist"));
                        
                        FTimerHandle TimerHandle;
                        AController* ExitingController = Exiting; // 필요하다면 로컬 복사
                        GetWorldTimerManager().SetTimer(TimerHandle, [this, ExitingController]()
                        {
                            Logout(ExitingController);
                        }, 3.f, false);
                        
                    }
                    
                }
            }

        
            /*if (AEGPlayerController* NewChief = Cast<AEGPlayerController>(APlayingPlayerStates[1].Pin()->GetOwner()))
            {
                bChiefPlayer = true;
                NewChief->ShowChiefPlayerUI();
                EG_LOG_ROLE(LogMS, Warning, TEXT("NewChiefasdf"));
            }*/
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
    FadeInScreen();

    GetWorldTimerManager().SetTimer(LevelChangeTimerHandle, this, &AEGLobbyGameModeBase::LevelChangeTimer, 2.0f, false);
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

void AEGLobbyGameModeBase::LevelChangeTimer()
{
    UEGGameInstance* EGGI = Cast<UEGGameInstance>(GetGameInstance());
    if (EGGI)
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("player: %d d"), GameState->PlayerArray.Num());
        EGGI->SetPlayerIndex(GameState->PlayerArray.Num());
        EGGI->ChangeLevel();     
    }
}

#pragma endregion
