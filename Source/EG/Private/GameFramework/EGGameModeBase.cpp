// Fill out your copyright notice in the Description page of Project Settings.

#include "EG/Public/GameFramework/EGGameModeBase.h"
#include "EngineUtils.h"
#include "EG/Public/GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "EG/Public/GameFramework/EGGameStateBase.h"
#include "Character/EGChickenCharacter.h"
#include "Character/Egg/EggActor.h"
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
            //RemainTime,
            10.0f,
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
            int32 SwapNum = FMath::RandRange(0, AInGameSpawnPoints.Num()-1);
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

    // player score calculate
    FinalPlayerScores.Sort([](const auto& A, const auto& B)
    {
        if (A.Value != B.Value)
            return A.Value > B.Value;
        return A.Key.IsValid() && B.Key.IsValid() && A.Key->PlayerIndex < B.Key->PlayerIndex;
    });
        
    if (AEGGameStateBase* GS = GetGameState<AEGGameStateBase>())
    {
        GS->SetFinalResults(FinalPlayerScores);
    }
    
    ClearStage();
}

void AEGGameModeBase::ClearStage()
{
    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("[why] World is invalid."));
    }

    //destroy all AI
    for (TActorIterator<AEGAICharacter> It(World); It; ++It)
    {
        AEGAICharacter* EGAI = *It;
        if (IsValid(EGAI))
        {
            EGAI->Destroy();
        }
    }

    for (TActorIterator<AEggActor> It(World); It; ++It)
    {
        AEggActor* EGG = *It;
        if (IsValid(EGG))
        {
            EGG->Destroy();
        }
    }
    
    //player goback playerstart
    UE_LOG(LogTemp, Warning, TEXT("before Loop"));
    int32 SpawnNum = 0;
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AEGPlayerController* PC = Cast<AEGPlayerController>(It->Get()))
        {
            if (AEGPlayerState* PS =  Cast<AEGPlayerState>(PC->PlayerState))
            {
                PS->RemoveEgg_Internal(PS->GetPlayerEggCount());
            }
            if (ACharacter* EGPlayerChar = Cast<AEGChickenCharacter>(PC->GetPawn()))
            {
                if (AEGPlayerStart** FoundStart = PlayerStartList.Find(SpawnNum))
                {
                    FVector StartLocation = (*FoundStart)->GetActorLocation();
                    EGPlayerChar->SetActorLocation(StartLocation);
                    SpawnNum++;
                }
            }
        }
    }
}
