// Fill out your copyright notice in the Description page of Project Settings.

#include "EG/Public/GameFramework/EG_GameModeBase.h" // 경로는 프로젝트 구조에 맞게
#include "EG/Public/GameFramework/EG_GameStateBase.h"
#include "EG/Public/Player/EG_PlayerController.h"
#include "EG/Public/Actor/EG_PlayerStart.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void AEG_GameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

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
}

void AEG_GameModeBase::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if (AEG_PlayerController* EGPC = Cast<AEG_PlayerController>(Exiting))
    {
        APlayingPlayerControllers.RemoveAll([EGPC](const TWeakObjectPtr<AEG_PlayerController>& P)
        {
            return !P.IsValid() || P.Get() == EGPC;
        });
    }
}

void AEG_GameModeBase::BeginPlay()
{
    Super::BeginPlay();
    InitializeSpawnPoint(); 
}

void AEG_GameModeBase::NotifyToAllPlayer(const FString& NotificationString)
{
    const FText Msg = FText::FromString(NotificationString);
    for (const TWeakObjectPtr<AEG_PlayerController>& WeakPC : APlayingPlayerControllers)
    {
        if (AEG_PlayerController* PC = WeakPC.Get())
        {
            PC->ClientNotifyMessage(Msg); 
        }
    }
}

void AEG_GameModeBase::InitializeSpawnPoint()
{
    SpawnPoints.Reset();

    UWorld* World = GetWorld();
    if (!World) return;

    for (TActorIterator<AEG_PlayerStart> It(World); It; ++It)
    {
        SpawnPoints.Add(*It);
    }

    SpawnPoints.Sort([](const AEG_PlayerStart* A, const AEG_PlayerStart* B)
    {
        const int32 LA = A ? A->GetPlayerIndex() : MAX_int32;
        const int32 LB = B ? B->GetPlayerIndex() : MAX_int32;
        return LA < LB;
    });
}

AActor* AEG_GameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{

    if (SpawnPoints.Num() == 0)
    {
        InitializeSpawnPoint();
        if (SpawnPoints.Num() == 0)
        {
            return Super::ChoosePlayerStart_Implementation(Player);
        }
    }

    int32 DesiredIndex = 0;

    if (Player && Player->PlayerState)
    {
 
        if (AGameStateBase* GS = GameState)
        {
            const TArray<APlayerState*>& Arr = GS->PlayerArray;
            const int32 FoundIdx = Arr.IndexOfByKey(Player->PlayerState);
            if (FoundIdx != INDEX_NONE)
            {
                DesiredIndex = FoundIdx;
            }
            else
            {
 
                DesiredIndex = FMath::Max(0, GetNumPlayers() - 1);
            }
        }
        else
        {
            DesiredIndex = FMath::Max(0, GetNumPlayers() - 1);
        }
    }
    else
    {
        DesiredIndex = FMath::Max(0, GetNumPlayers() - 1);
    }

    DesiredIndex %= FMath::Max(1, SpawnPoints.Num());


    for (int32 i = 0; i < SpawnPoints.Num(); ++i)
    {
        const int32 TryIdx = (DesiredIndex + i) % SpawnPoints.Num();
        if (AEG_PlayerStart* PS = SpawnPoints[TryIdx])
        {
            if (IsSpawnPointAvailable(PS))
            {
                return PS;
            }
        }
    }

    return Super::ChoosePlayerStart_Implementation(Player);
}

bool AEG_GameModeBase::IsSpawnPointAvailable(const AActor* Start) const
{
    if (!Start) return false;

    const FVector Loc = Start->GetActorLocation();
    const float Radius = 80.f; // 캡슐 반지름과 여유분에 맞게 조정

    TArray<FOverlapResult> Hits;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(SpawnCheck), /*bTraceComplex=*/false);
    FCollisionObjectQueryParams ObjParams(FCollisionObjectQueryParams::AllDynamicObjects);

    const bool bAny = GetWorld()->OverlapMultiByObjectType(
        Hits,
        Loc,
        FQuat::Identity,
        ObjParams,
        FCollisionShape::MakeSphere(Radius),
        Params
    );

    if (bAny)
    {
        for (const FOverlapResult& R : Hits)
        {
            if (AActor* A = R.GetActor())
            {
                if (A->IsA<APawn>())
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void AEG_GameModeBase::GameStart()
{ 
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AEG_PlayerController* PC = Cast<AEG_PlayerController>(It->Get()))
        {
            PC->ClientGameStarted();
        }
    }
}
