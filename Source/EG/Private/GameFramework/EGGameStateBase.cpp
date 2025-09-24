// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/EGGameStateBase.h"
#include "EG/EGLog.h"
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
	DOREPLIFETIME(AEGGameStateBase, LeaderboardSnapshot); //kms
	DOREPLIFETIME(AEGGameStateBase, RoundAward);		//kms
}

void AEGGameStateBase::OnRep_RemainingCountdown()
{
	EG_LOG_NET(LogJM, Log, TEXT("Countdown updated: %d"), RemainingCountdown);

	OnCountdownUpdated.Broadcast(RemainingCountdown);
}

void AEGGameStateBase::OnRep_RemainingPlayTime()
{
    UE_LOG(LogTemp, Log, TEXT("Play Time: %d"), RemainingPlayTime);

	OnPlayTimeUpdated.Broadcast(RemainingPlayTime);    
}

// (작성자 : KMS)
void AEGGameStateBase::OnRep_Leaderboard()
{
	UE_LOG(LogTemp, Log, TEXT("Leaderboard updated, entries: %d"), LeaderboardSnapshot.Num());
}

void AEGGameStateBase::OnRep_Award()
{
	UE_LOG(LogTemp, Log, TEXT("Award replicated -> WinnerIndex: %d, Score: %d"),
		RoundAward.PlayerIndex,
		RoundAward.WinnerEggScore);
	// ui 출력
}

void AEGGameStateBase::UpdateLeaderboard()
{
	TArray<APlayerState*> SortedPlayers = PlayerArray;
	SortedPlayers.Sort([](const APlayerState& A, const APlayerState& B) {
		const AEGPlayerState& PA = static_cast<const AEGPlayerState&>(A);
		const AEGPlayerState& PB = static_cast<const AEGPlayerState&>(B);
		return PA.GetPlayerEggCount() > PB.GetPlayerEggCount();
	});

	TArray<FAward> NewSnapshot;
	for (APlayerState* PS : SortedPlayers)
	{
		if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(PS))
		{
			if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(EGPS->GetOwner()))
			{
				FAward Entry;
				Entry.PlayerIndex     = EGPC->PlayerIndex;            // 컨트롤러에 부여한 인덱스
				Entry.WinnerEggScore  = EGPS->GetPlayerEggCount();  // 플레이어 상태에서 알 개수
				NewSnapshot.Add(Entry);
			}
		}
	}

	// 순위 변동이 있을 때만 replicate
	if (LeaderboardSnapshot != NewSnapshot)
	{
		LeaderboardSnapshot = NewSnapshot;
	}
}

void AEGGameStateBase::FinalizeAward()
{
	if (HasAuthority())
	{
		if (LeaderboardSnapshot.Num() > 0)
		{
			RoundAward.PlayerIndex     = LeaderboardSnapshot[0].PlayerIndex;
			RoundAward.WinnerEggScore  = LeaderboardSnapshot[0].WinnerEggScore;

			UE_LOG(LogTemp, Log, TEXT("FinalizeAward -> WinnerIndex: %d, Score: %d"),
				RoundAward.PlayerIndex,
				RoundAward.WinnerEggScore);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FinalizeAward called with empty leaderboard!"));
		}
	}
}

void AEGGameStateBase::SetFinalResults(const TArray<TPair<TWeakObjectPtr<AEGPlayerController>, int32>>& Scores)
{
	// LeaderboardSnapshot 채우고 replicate
	LeaderboardSnapshot.Empty();
	for (const auto& Pair : Scores)
	{
		if (Pair.Key.IsValid())
		{
			if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(Pair.Key.Get()))
			{
				if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(EGPC->PlayerState))
				{
					FAward Entry;
					Entry.PlayerIndex    = EGPC->PlayerIndex;         
					Entry.WinnerEggScore = EGPS->GetPlayerEggCount(); 
					LeaderboardSnapshot.Add(Entry);
				}
			}
		}
	}

	if (LeaderboardSnapshot.Num() > 0)
	{
		RoundAward.PlayerIndex = 0;
		RoundAward.WinnerEggScore = LeaderboardSnapshot[0].WinnerEggScore;
	}
}