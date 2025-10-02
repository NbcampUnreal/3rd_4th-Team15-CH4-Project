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
		RoundAward.PlayerIndex,
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
	TArray<FAward> NewSnapshot = LeaderboardSnapshot;

	// 현재 존재하는 PlayerState들을 확인해서 갱신
	for (APlayerState* PS : PlayerArray)
	{
		if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(PS))
		{
			if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(EGPS->GetOwner()))
			{
				int32 PlayerIdx = EGPC->PlayerIndex;

				// Snapshot에 이미 존재하는지 확인
				FAward* Found = NewSnapshot.FindByPredicate([PlayerIdx](const FAward& Entry) {
					return Entry.PlayerIndex == PlayerIdx;
				});

				if (Found)
				{
					// 점수 갱신
					Found->PlayerEggScore = EGPS->GetPlayerEggCount();
				}
				else
				{
					// 새로 추가 (처음 들어온 경우)
					FAward Entry;
					Entry.PlayerIndex    = PlayerIdx;
					Entry.PlayerEggScore = EGPS->GetPlayerEggCount();
					NewSnapshot.Add(Entry);
				}
			}
		}
	}

	// 점수 높은 순서대로 정렬
	NewSnapshot.Sort([](const FAward& A, const FAward& B) {
		return A.PlayerEggScore > B.PlayerEggScore;
	});

	// 변경이 있을 때만 적용
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
			RoundAward.PlayerEggScore  = LeaderboardSnapshot[0].PlayerEggScore;

			UE_LOG(LogTemp, Log, TEXT("FinalizeAward -> WinnerIndex: %d, Score: %d"),
				RoundAward.PlayerIndex,
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
			if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(Pair.Key.Get()))
			{
				if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(EGPC->PlayerState))
				{
					FAward Entry;
					Entry.PlayerIndex    = EGPC->PlayerIndex;         
					Entry.PlayerEggScore = EGPS->GetPlayerEggCount(); 
					LeaderboardSnapshot.Add(Entry);
				}
			}
		}
	}

	if (LeaderboardSnapshot.Num() > 0)
	{
		RoundAward.PlayerIndex = 0;
		RoundAward.PlayerEggScore = LeaderboardSnapshot[0].PlayerEggScore;
	}
}