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
                Entry.PlayerIndex    = UniqueId;
                Entry.PlayerEggScore = 0;
                EGGS->LeaderboardSnapshot.Add(Entry);
            }
        }
    }
    
    SetRoomLeader();
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
    }
    else
    {
        EG_LOG_ROLE(LogMS, Warning, TEXT("Game Start Fail, you are not RoomLeader"));
    }
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
        }
    }

    // player score sort
    FinalPlayerScores.Sort([](const auto& A, const auto& B)
  {
      if (A.Value != B.Value)
          return A.Value > B.Value;
      return A.Key.IsValid() && B.Key.IsValid() && A.Key->PlayerIndex < B.Key->PlayerIndex;
  });
   
    if (AEGGameStateBase* EGGS = GetGameState<AEGGameStateBase>())
    {
        EGGS->SetFinalResults(FinalPlayerScores);
        EGGS->FinalizeAward();
    }
    ClearStage();

	GetWorld()->ServerTravel("/Game/UI/Map/Lobby?listen");  // 작성자: 김효영
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

// ä�� (�ۼ��� : ��ȿ��)
#pragma region Chatting
void AEGGameModeBase::SendChatMessage(const FString& Message)
{
    // ��� �÷��̾� ��Ʈ�ѷ��� ���� Iterator�� �̿�
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
