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

    GetWorldTimerManager().SetTimer(
    TimerHandle,
    this,
    &AEGGameModeBase::GameStart, 
    10.0f,
    false 
    );
}
/*
void AEGGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
    FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
    
    const bool bMatchStarted = bGameStarted || HasMatchStarted();

    if (bMatchStarted)
    {
        ErrorMessage = TEXT("MatchInProgress"); // 비어 있으면 허용, 채우면 거절
        return;
    }
}
*/

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

    /*
    if (AEG_GameStateBase* GS = GetGameState<AEG_GameStateBase>())
    {

    }

    if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(NewPlayer))
    {
        APlayingPlayerControllers.Add(EGPC);

        EGPC->ClientNotifyMessage(FText::FromString(TEXT("Connected to the game server.")));
    }

    if (GetNumPlayers() > 1)
    {
        GameStart();
    }

    if (!bGameStarted && GetNumPlayers() > 1)
    {
        bGameStarted = true;
        GameStart();
    }
    */
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
        
    EG_LOG_ROLE(LogMS, Warning, TEXT("No SpawnPoint found for index %d, using Super."));
    return Super::ChoosePlayerStart_Implementation(Player);
}


void AEGGameModeBase::GameStart()
{
    GameOver();
    
    if (GetNumPlayers() > 1)
    {
        UWorld* World = GetWorld();
        
        AInGameSpawnPoints.Empty();
        for (TActorIterator<AEGInGameSpawnPoints> It(World); It; ++It)
        {
            AInGameSpawnPoints.Add(*It);
        }

        // random suffle
        for (int32 i = 0; i < AInGameSpawnPoints.Num(); i++)
        {
            int32 SwapIdx = FMath::RandRange(0, AInGameSpawnPoints.Num()-1);
            AInGameSpawnPoints.Swap(i, SwapIdx);
        }

        //player teleport
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
                        FRotator SpawnRotation(0.f , FMath::RandRange(0, 360), 0.f);
                        PC->SetControlRotation(SpawnRotation);
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
                /*
                EG_LOG_ROLE(LogMS, Warning, TEXT("Ai spawn at : %d"), 
                    AInGameSpawnPoints[k]->GetSpawnSortNum());
                */
                FVector SpawnLocation = AInGameSpawnPoints[k]->GetActorLocation();
                FRotator SpawnRotation(0.f , FMath::RandRange(0, 360), 0.f);

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
}


void AEGGameModeBase::GameOver()
{
    if (!GetWorld())
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("GameOver called with invalid World!"));
        return;
    }

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
        auto FirstPlayer = FinalPlayerScores[0].Key;
        if (FirstPlayer.IsValid())
        {
            EG_LOG_ROLE(LogMS, Warning, TEXT("Player: %d is winnder, Score: %d"), 
                FirstPlayer->PlayerIndex, 
                FinalPlayerScores[0].Value);
        }
    }
}
