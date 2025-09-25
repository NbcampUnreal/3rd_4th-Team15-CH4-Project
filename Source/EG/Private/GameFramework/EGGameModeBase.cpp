// Fill out your copyright notice in the Description page of Project Settings.

#include "EG/Public/GameFramework/EGGameModeBase.h"
#include "EngineUtils.h"
#include "EG/Public/GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "EG/Public/GameFramework/EGGameStateBase.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/EGInGameSpawnPoints.h"
#include "GameFramework/EGPlayerState.h"


void AEGGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    //StartCount();
}

void AEGGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
    FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

    if (APlayingPlayerControllers.Num() >= 6)
    {
        ErrorMessage = TEXT("ServerError_MaxPlayersReached");
        return;
    }
}


void AEGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    static int32 NextIndex = 0;

    if (AEGPlayerController* PC = Cast<AEGPlayerController>(NewPlayer))
    {
        PC->SetPlayerIndex(NextIndex);
        NextIndex++;
    }
    else
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("player %d cant join here."), NextIndex);
    }

    if (APlayingPlayerControllers.Num() > 6)
    {
        UE_LOG(LogTemp, Warning, TEXT("Too many players! Kicking..."));

        // map deport
        // NewPlayer->ClientTravel(TEXT("/Game/Maps/MainMenu"), TRAVEL_Absolute);
        NewPlayer->Destroy();
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
    
    int32 PlayerIndex = GetNumPlayers()-1;
        
        if (AEGPlayerStart** FoundStart = PlayerStartList.Find(PlayerIndex))
        {
            return *FoundStart;
        }
        
    EG_LOG_ROLE(LogMS, Warning, TEXT("No SpawnPoint found for index %d, using Super."), PlayerIndex);
    return Super::ChoosePlayerStart_Implementation(Player);
}


void AEGGameModeBase::StartCount()
{
    GetWorldTimerManager().SetTimer(
    GameStartingTimerHandle,
    this,
    &AEGGameModeBase::GameStart, 
    10.0f,
    false 
    );
}

void AEGGameModeBase::EndCount()
{
    AEGGameStateBase* GS = GetGameState<AEGGameStateBase>();
    if (IsValid(GS))
    {
        float RemainTime = GS->RemainingPlayTime;

        GetWorldTimerManager().SetTimer(
            GameEndTimerHandle,
            this,
            &AEGGameModeBase::GameOver,
            RemainTime,
            false
        );
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GameState is not valid in BeginPlay!"));
    }
}


void AEGGameModeBase::GameStart()
{       
    if (GetNumPlayers() > 1)
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("Game Start"));
        UWorld* World = GetWorld();
        
        AInGameSpawnPoints.Empty();
        for (TActorIterator<AEGInGameSpawnPoints> It(World); It; ++It)
        {
            AInGameSpawnPoints.Add(*It);
        }

        for (int32 i = 0; i < AInGameSpawnPoints.Num(); i++)
        {
            int32 SwapIdx = FMath::RandRange(0, AInGameSpawnPoints.Num()-1);
            AInGameSpawnPoints.Swap(i, SwapIdx);
        }

        int32 SpawnIdx = 0;
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (AEGPlayerController* PC = Cast<AEGPlayerController>(It->Get()))
            {
                if (ACharacter* ChickChar = Cast<AEGChickenCharacter>(PC->GetPawn()))
                {
                    if (AInGameSpawnPoints.IsValidIndex(SpawnIdx))
                    {
                        FVector StartLocation = AInGameSpawnPoints[SpawnIdx]->GetActorLocation();
                        ChickChar->SetActorLocation(StartLocation);
                        SpawnIdx++;
                    }
                }
            }
        }
        for (int k = SpawnIdx; k < AInGameSpawnPoints.Num(); k++)
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
    EndCount();
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
                FinalPlayerScores.Add(TPair<TWeakObjectPtr<AEGPlayerController>, int32>(PC, PS->GetPlayerEggCount()));
            }
            else
            {
                EG_LOG_ROLE(LogMS, Warning, TEXT("Invalid PlayerState for PlayerController"));
            }
        }
    }

    FinalPlayerScores.Sort([](const auto& A, const auto& B)
    {
        if (A.Value != B.Value)
            return A.Value > B.Value;
        return A.Key.IsValid() && B.Key.IsValid() && A.Key->PlayerIndex < B.Key->PlayerIndex;
    });

    if (FinalPlayerScores.IsValidIndex(0))
    {
        int32 MaxScore = FinalPlayerScores[0].Value;

        for (const auto& Pair : FinalPlayerScores)
        {
            if (Pair.Value == MaxScore)
            {
                auto Player = Pair.Key;
                if (Player.IsValid())
                {
                    EG_LOG_ROLE(LogMS, Warning, TEXT("Player: %d is winner, Score: %d"),
                        Player->PlayerIndex,
                        Pair.Value);
                }
            }
            else
            {                               
                break;
            }
        }
    }
}


// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
void AEGGameModeBase::ChangeLevel(const FString& MapName)
{
    if (HasAuthority()) // 서버에서만
    {
        FString Command = FString::Printf(TEXT("/Game/UI/Map/%s?listen"), *MapName);
        GetWorld()->ServerTravel(Command);
    }
}

#pragma endregion

// 채팅 (작성자 : 김효영)
#pragma region Chatting
void AEGGameModeBase::SendChatMessage(const FString& Message)
{
    // 모든 플레이어 컨트롤러에 대한 Iterator을 이용
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AEGPlayerController* EGPC = Cast<AEGPlayerController>(*It);
        if (EGPC)
        {
            EGPC->ClientAddChatMessage(Message);
        }
    }
}

#pragma endregion