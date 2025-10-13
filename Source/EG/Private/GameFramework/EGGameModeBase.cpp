// Fill out your copyright notice in the Description page of Project Settings.
#include "EG/Public/GameFramework/EGGameModeBase.h"
#include "EngineUtils.h"
#include "EG/Public/GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "EG/Public/GameFramework/EGGameStateBase.h"
#include "Character/EGChickenCharacter.h"
#include "Character/Egg/EggPoolManagerSubsystem.h"
#include "GameFramework/EGInGameSpawnPoints.h"
#include "GameFramework/EGPlayerState.h"
#include "GameFramework/EGGameInstance.h"


void AEGGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
    {
        playerCount = GI->GetPlayerIndex();
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

AEGGameModeBase::AEGGameModeBase()
{
    bUseSeamlessTravel = true;
}

void AEGGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
    
    ErrorMessage = TEXT("ServerError_MaxPlayersReached");
    return;
}

void AEGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UE_LOG(LogTemp, Warning, TEXT("PostLogin PlayerID: %d"), 
    NewPlayer->GetPlayerState<APlayerState>() ? NewPlayer->GetPlayerState<APlayerState>()->GetPlayerId() : -1);
    
    if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(NewPlayer->PlayerState))
    {
        if (!APlayingPlayerState.Contains(EGPS))
        {
            APlayingPlayerState.Add(EGPS);
        }
        
        if (AEGGameStateBase* EGGS = GetGameState<AEGGameStateBase>())
        {
            FAward Entry;
            Entry.PlayerID = EGPS->GetPlayerId();
            Entry.PlayerEggScore = 0;
            EGGS->LeaderboardSnapshot.Add(Entry);
            
            if (playerCount == currentPlayerCount)
            {
                //GameStart();
            }
        }        
    }
}

void AEGGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
    EG_LOG_ROLE(LogMS, Warning, TEXT("HandleSeamlessTravelPlayer Start"));

    Super::HandleSeamlessTravelPlayer(C);

    currentPlayerCount++;

    if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
    {
        // 모든 플레이어가 로드 완료했다고 판단되면 StartGame
        if (currentPlayerCount == playerCount)
        {
            GameStart();
        }
    }
}

void AEGGameModeBase::Logout(AController* Exiting)
{
    if (APlayerController* PC = Cast<APlayerController>(Exiting))
    {
        if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(PC->PlayerState))
        {
            EG_LOG_ROLE(LogMS, Warning, TEXT("player %d logout."), EGPS->GetPlayerId());

            APlayingPlayerState.RemoveAll([EGPS](const TWeakObjectPtr<AEGPlayerState>& P)
            {
                return !P.IsValid() || P.Get() == EGPS;
            });
            
            EG_LOG_ROLE(LogMS, Warning, TEXT("Current Player Count: %d"), APlayingPlayerState.Num());
        }
    }

    Super::Logout(Exiting);
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
    int32 PlayerNum = GetNumPlayers();
        if (AEGPlayerStart** FoundStart = PlayerStartList.Find(PlayerNum))
        {
            return *FoundStart;
        }
    EG_LOG_ROLE(LogMS, Warning, TEXT("No SpawnPoint found for index %d, using Super."), PlayerNum);
    return Super::ChoosePlayerStart_Implementation(Player);
}

void AEGGameModeBase::GameStart()
{
    if (GetNumPlayers() > 1)
    {
        if (AEGGameStateBase* EGGS = GetGameState<AEGGameStateBase>())
        {
            EGGS->StartCountdown();
        }
        EG_LOG_ROLE(LogMS, Warning, TEXT("Game Start"));
        UWorld* World = GetWorld();
        AInGameSpawnPoints.Empty();
        for (TActorIterator<AEGInGameSpawnPoints> It(World); It; ++It)
        {
            AInGameSpawnPoints.Add(*It);
        }
        for (int32 i = 0; i < AInGameSpawnPoints.Num(); i++)
        {
            int32 SwapNum = FMath::RandRange(0, AInGameSpawnPoints.Num() - 1);
            AInGameSpawnPoints.Swap(i, SwapNum);
        }
        int32 SpawnNum = 0;
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (AEGPlayerController* PC = Cast<AEGPlayerController>(It->Get()))
            {
                if (ACharacter* EGPlayerChar = Cast<AEGChickenCharacter>(PC->GetPawn()))
                {
                    if (AInGameSpawnPoints.IsValidIndex(SpawnNum))
                    {
                        FVector StartLocation = AInGameSpawnPoints[SpawnNum]->GetActorLocation();
                        EGPlayerChar->SetActorLocation(StartLocation);
                        SpawnNum++;
                    }
                }

                // JM : 게임 시작 시 SFX 재생
                PC->ClientRPC_PlaySFXGameStart();
            }
        }
        for (int k = SpawnNum; k < AInGameSpawnPoints.Num(); k++)
        {
            if (AInGameSpawnPoints[k].IsValid())
            {
                FVector SpawnLocation = AInGameSpawnPoints[k]->GetActorLocation();
                FRotator SpawnRotation(0.f, AInGameSpawnPoints[k]->GetActorRotation().Yaw, 0.f);
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.Instigator = GetInstigator();
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
                AEGAICharacter* SpawnedActor = GetWorld()->SpawnActor<AEGAICharacter>(AICharacter, SpawnLocation, SpawnRotation, SpawnParams);
                if (SpawnedActor)
                {
                    EG_LOG_ROLE(LogMS, Warning, TEXT("ai Spawn"));
                }
                else
                {
                    EG_LOG_ROLE(LogMS, Warning, TEXT("ai Spawn fail"));
                }
            }
        }
        EG_LOG_ROLE(LogMS, Warning, TEXT("-------------------------------------"));

    }

    FadeOutScreen(); // 게임 시작 시 페이드 아웃 (작성자 : 김세훈) 
}

void AEGGameModeBase::GameOver()
{
    EG_LOG_ROLE(LogMS, Warning, TEXT("Game Over"));
    
    TArray<TPair<TWeakObjectPtr<AEGPlayerController>, int32>> FinalPlayerScores;

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AEGPlayerController* PC = Cast<AEGPlayerController>(It->Get()))
        {
            if (AEGPlayerState* PS = Cast<AEGPlayerState>(PC->PlayerState))
            {
                FinalPlayerScores.Add({ PC, PS->GetPlayerEggCount() });
            }
            // JM : 게임 종료 시 SFX 재생
            PC->ClientRPC_PlaySFXGameOver();
        }
    }
    
    FinalPlayerScores.Sort([](const auto& A, const auto& B)
    {
        return A.Value > B.Value;
    });
    int32 TopScore = (FinalPlayerScores.Num() > 0) ? FinalPlayerScores[0].Value : 0;

    TArray<FFinalResult> FinalResults;

    for (const auto& Pair : FinalPlayerScores)
    {
        if (!Pair.Key.IsValid()) continue;

        if (AEGPlayerState* PS = Pair.Key->GetPlayerState<AEGPlayerState>())
        {
            FFinalResult Result;
            Result.PlayerId = PS->GetPlayerId();
            Result.bIsWinner = (Pair.Value == TopScore);
            FinalResults.Add(Result);
            
            EG_LOG(LogKH, Log, TEXT("player %d score %d : %s"), PS->GetPlayerId(), Pair.Value, Result.bIsWinner ? TEXT("Winner") : TEXT("Loser"));
        }
    }

    if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
    {
        GI->SetFinalResults(FinalResults);
    }

    // GetWorld()->ServerTravel(TEXT("/Game/OhMyEgg/Sequence/L_Sequence?listen"), true);
    FTimerHandle LevelTravelTimerHandle;
    GetWorldTimerManager().SetTimer(LevelTravelTimerHandle, this, &AEGGameModeBase::TravelToLevel, 1.0f, false);

    FadeInScreen();
}

void AEGGameModeBase::TravelToLevel()
{
    EG_LOG(LogJM, Log, TEXT("Start"));
    GetWorld()->ServerTravel(TEXT("/Game/OhMyEgg/Sequence/L_Sequence?listen"), true);
    EG_LOG(LogJM, Log, TEXT("End"));
}

// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
void AEGGameModeBase::FadeInScreen()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(It->Get()))
        {
            EGPC->ClientRPCFadeInScreen();
        }
    }
}

void AEGGameModeBase::FadeOutScreen()
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
