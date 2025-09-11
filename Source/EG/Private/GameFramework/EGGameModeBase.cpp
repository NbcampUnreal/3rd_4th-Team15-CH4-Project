// Fill out your copyright notice in the Description page of Project Settings.

#include "EG/Public/GameFramework/EGGameModeBase.h"
#include "EngineUtils.h"
#include "EG/Public/GameFramework/EGPlayerStart.h"
#include "EGLog.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/EGInGameSpawnPoints.h"


void AEGGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
    MyTimerHandle,
    this,
    &AEGGameModeBase::GameStart, 
    5.0f,
    true 
    );
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
        UE_LOG(LogTemp, Warning, TEXT("[why] World is invalid. Abort building PlayerStartList."));
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

    if (AEGPlayerController* PC = Cast<AEGPlayerController>(Player))
    {
        int32 PlayerIndex = GetNumPlayers()-1; //PC->PlayerIndex or PlayerController index
        
        if (AEGPlayerStart** FoundStart = PlayerStartList.Find(PlayerIndex))
        {
            return *FoundStart;
        }
    }
    
    EG_LOG_ROLE(LogMS, Warning, TEXT("No SpawnPoint found for index %d, using Super."));
    return Super::ChoosePlayerStart_Implementation(Player);
}


void AEGGameModeBase::GameStart()
{
    if (GetNumPlayers() > 1)
    {
        UWorld* World = GetWorld();
        
        for (TActorIterator<AEGInGameSpawnPoints> It(World); It; ++It)
        {
            AInGameSpawnPoints.Add(*It);
            //AInGameSpawnPoints.Init(10, 5);
        }
        
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (AEGPlayerController* PC = Cast<AEGPlayerController>(It->Get()))
            {
                if (ACharacter* ChickChar = Cast<AEGChickenCharacter>(PC->GetPawn()))
                {
                    int32 RandomInt = FMath::RandRange(1, AInGameSpawnPoints.Num()-1);
                    if (AInGameSpawnPoints[RandomInt].IsValid())
                    {
                        FVector StartLocation = AInGameSpawnPoints[RandomInt]->GetActorLocation();
                        FRotator StartRotation = FRotator::ZeroRotator;
                        ChickChar->SetActorLocationAndRotation(StartLocation, StartRotation);
                    }                
                }
                //PC->ClientGameStarted();
            }
        }        
    }
    
    
}
