// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/EGGameStateBase.h"

#include <Manager/EGDelegateManager.h>

#include "EG/EGLog.h"
#include "GameFramework/EGGameModeBase.h"
#include "GameFramework/EGPlayerState.h"
#include "Net/UnrealNetwork.h"

void AEGGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	EG_LOG_NET(LogJM, Log, TEXT(""));

	DOREPLIFETIME(ThisClass, AlivePlayerControllerCount);
	DOREPLIFETIME(ThisClass, MatchState);
	DOREPLIFETIME(ThisClass, RemainingCountdown);
	DOREPLIFETIME(ThisClass, RemainingPlayTime);
	DOREPLIFETIME(ThisClass, LeaderboardSnapshot); //kms
	DOREPLIFETIME(ThisClass, RoundAward);		//kms
}

void AEGGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	DelegateManager = GetGameInstance()->GetSubsystem<UEGDelegateManager>();
}


void AEGGameStateBase::OnRep_RemainingCountdown()
{
	EG_LOG_NET(LogJM, Log, TEXT("Countdown updated: %d"), RemainingCountdown);
	
	DelegateManager->OnCountdownUpdated.Broadcast(RemainingCountdown);
}

void AEGGameStateBase::OnRep_RemainingPlayTime()
{
    UE_LOG(LogTemp, Log, TEXT("Play Time: %d"), RemainingPlayTime);
	DelegateManager->OnPlayTimeUpdated.Broadcast(RemainingPlayTime);
}

// (작성자 : KMS)
void AEGGameStateBase::OnRep_Leaderboard()
{
	// 리더보드 데이터가 복제될 때 클라이언트에서 호출됩니다.

	// 1. GameInstanceSubsystem인 EGDelegateManager를 가져옵니다.
	if (const UWorld* World = GetWorld())
	{
		if (const UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UEGDelegateManager* LocalDelegateManager = GameInstance->GetSubsystem<UEGDelegateManager>())
			{
				// 2. UI 갱신용 델리게이트를 브로드캐스트합니다.
				//    클라이언트의 UI는 이 이벤트를 받아 순위표를 갱신합니다.
				LocalDelegateManager->OnLeaderboardUpdated.Broadcast(LeaderboardSnapshot);
			}
		}
	}

	// **혹은** DelegateManager의 BroadcastPlayerState 함수를 사용하도록 설계되었다면 해당 함수를 호출합니다.
	// BroadcastPlayerState(this); // GameState를 인자로 넘겨 리더보드 데이터를 브로드캐스트하도록 구현되어 있을 경우
}

void AEGGameStateBase::OnRep_Award()
{
	UE_LOG(LogTemp, Log, TEXT("Award replicated -> WinnerIndex: %d, Score: %d"),
		RoundAward.PlayerID,
		RoundAward.PlayerEggScore);
	
	DelegateManager->OnAwardUpdated.Broadcast(RoundAward);

}

void AEGGameStateBase::CheckRoomLeader()
{
	//UE_LOG(LogTemp, Log, TEXT("StartCountdown called with UniqueID: %d"), UniqueID);
	if (AEGGameModeBase* EGGM = Cast<AEGGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		EGGM->GameStart();
		//UE_LOG(LogTemp, Log, TEXT("Try to Game Start player : %d"), UniqueID);
	}
}

void AEGGameStateBase::StartCountdown()
{
	if (HasAuthority())
	{
		RemainingCountdown = RemainingPlayTime;
		
		GetWorld()->GetTimerManager().SetTimer(
			CountdownTimerHandle,
			this,
			&AEGGameStateBase::DecrementCountdown,
			1.0f,
			true
		);
	}
}

void AEGGameStateBase::DecrementCountdown()
{
	if (HasAuthority() && RemainingCountdown > 0)
	{
		RemainingCountdown--;
        
		if (RemainingCountdown <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
			
			if (AEGGameModeBase* EGGM = Cast<AEGGameModeBase>(GetWorld()->GetAuthGameMode()))
			{
				EGGM->GameOver();
			}
		}
	}
}


void AEGGameStateBase::UpdateLeaderboard()
{
	// 새 스냅샷을 무조건 전체 PlayerArray 기준으로 만든다
	TArray<FAward> NewSnapshot;

	for (APlayerState* PS : PlayerArray)
	{
		if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(PS))
		{
			FAward Entry;
			Entry.PlayerID       = EGPS->GetPlayerID();
			Entry.PlayerEggScore = EGPS->GetPlayerEggCount();
			NewSnapshot.Add(Entry);
		}
	}

	// 점수 높은 순서 + 동점자는 ID 낮은 순서
	NewSnapshot.Sort([](const FAward& A, const FAward& B) {
		return (A.PlayerEggScore == B.PlayerEggScore)
			? (A.PlayerID < B.PlayerID)
			: (A.PlayerEggScore > B.PlayerEggScore);
	});

	// 달라졌을 때만 리플리케이트
	if (LeaderboardSnapshot != NewSnapshot)
	{
		LeaderboardSnapshot = MoveTemp(NewSnapshot);
		OnRep_Leaderboard(); // 서버에서도 즉시 Broadcast 해주고 싶으면 직접 호출
	}
}

void AEGGameStateBase::FinalizeAward()
{
	if (HasAuthority())
	{
		if (LeaderboardSnapshot.Num() > 0)
		{
			RoundAward.PlayerID     = LeaderboardSnapshot[0].PlayerID;
			RoundAward.PlayerEggScore  = LeaderboardSnapshot[0].PlayerEggScore;

			UE_LOG(LogTemp, Log, TEXT("FinalizeAward -> WinnerIndex: %d, Score: %d"),
				RoundAward.PlayerID,
				RoundAward.PlayerEggScore);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FinalizeAward called with empty leaderboard!"));
		}
	}
}

void AEGGameStateBase::SetFinalResults(const TArray<TPair<TWeakObjectPtr<AEGPlayerController>, int32>>& Scores)
{
	LeaderboardSnapshot.Empty();
	for (const auto& Pair : Scores)
	{
		if (Pair.Key.IsValid())
		{
			if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(Pair.Key->PlayerState))

				{
					FAward Entry;
					Entry.PlayerID    = EGPS->GetPlayerID();         
					Entry.PlayerEggScore = EGPS->GetPlayerEggCount(); 
					LeaderboardSnapshot.Add(Entry);
				}
		}
	}

	if (LeaderboardSnapshot.Num() > 0)
	{
		RoundAward.PlayerID = 0;
		RoundAward.PlayerEggScore = LeaderboardSnapshot[0].PlayerEggScore;
	}
}