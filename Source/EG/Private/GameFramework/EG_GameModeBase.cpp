// Fill out your copyright notice in the Description page of Project Settings.

#include "EngineUtils.h"

#include "EG/Public/GameFramework/EG_GameModeBase.h"
#include "EG/Public/GameFramework/EG_PlayerStart.h"
#include "EGLog.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/EG_InGameSpawnPoints.h"


void AEG_GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
    MyTimerHandle,
    this,
    &AEG_GameModeBase::GameStart, 
    5.0f,
    true 
    );
}

void AEG_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    static int32 NextIndex = 0;

    if (AEG_PlayerController* PC = Cast<AEG_PlayerController>(NewPlayer))
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

    if (AEG_PlayerController* EGPC = Cast<AEG_PlayerController>(NewPlayer))
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



void AEG_GameModeBase::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if (AEG_PlayerController* EGPC = Cast<AEG_PlayerController>(Exiting))
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("player %d logout."), EGPC->PlayerIndex);
        APlayingPlayerControllers.RemoveAll([EGPC](const TWeakObjectPtr<AEG_PlayerController>& P)
        {
            return !P.IsValid() || P.Get() == EGPC;
        });
    }
}


void AEG_GameModeBase::InitializeSpawnPoint()
{
    PlayerStartList.Empty();

    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        UE_LOG(LogTemp, Warning, TEXT("[why] World is invalid. Abort building PlayerStartList."));
    }
	
    for (TActorIterator<AEG_PlayerStart> It(World); It; ++It)
    {
        AEG_PlayerStart* PS = *It;
        if (!IsValid(PS))
        {
            continue;
        }
        PlayerStartList.Add(PS->GetPlayerIndex(), PS);
    }
}

AActor* AEG_GameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    InitializeSpawnPoint();

    if (AEG_PlayerController* PC = Cast<AEG_PlayerController>(Player))
    {
        int32 PlayerIndex = GetNumPlayers()-1; //PC->PlayerIndex or PlayerController index
        
        if (AEG_PlayerStart** FoundStart = PlayerStartList.Find(PlayerIndex))
        {
            return *FoundStart;
        }
    }
    
    EG_LOG_ROLE(LogMS, Warning, TEXT("No SpawnPoint found for index %d, using Super."));
    return Super::ChoosePlayerStart_Implementation(Player);
}


void AEG_GameModeBase::GameStart()
{
    if (GetNumPlayers() > 1)
    {
        UWorld* World = GetWorld();
        
        for (TActorIterator<AEG_InGameSpawnPoints> It(World); It; ++It)
        {
            AInGameSpawnPoints.Add(*It);
            //AInGameSpawnPoints.Init(10, 5);
        }
        
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (AEG_PlayerController* PC = Cast<AEG_PlayerController>(It->Get()))
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
