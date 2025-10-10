// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "EGPlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/DelegateCombinations.h"
#include "EGGameStateBase.generated.h"

// (작성자 : KMS)
USTRUCT(BlueprintType)
struct FAward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerEggScore;

	FAward()
	   : PlayerID(0)
	   , PlayerEggScore(0)
	{}

	bool operator==(const FAward& Other) const
	{
		return PlayerID == Other.PlayerID
			&& PlayerEggScore == Other.PlayerEggScore;
	}
};
// 여기까지 KMS

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	None,
	Waiting,
	Countdown,
	Playing,
	Ending,
	End
};



UCLASS()
class EG_API AEGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 AlivePlayerControllerCount = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	EMatchState MatchState = EMatchState::Waiting;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingCountdown, VisibleAnywhere, BlueprintReadOnly)
	int32 RemainingCountdown = 100;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingPlayTime, VisibleAnywhere, BlueprintReadOnly)
	int32 RemainingPlayTime = 30;
	
	UPROPERTY()
	class UEGDelegateManager* DelegateManager;
	
	/// (작성자 : KMS)
	FTimerHandle CountdownTimerHandle;
	
	UPROPERTY(ReplicatedUsing=OnRep_Leaderboard)
	TArray<FAward> LeaderboardSnapshot;
	
	UPROPERTY(ReplicatedUsing=OnRep_Award)
	FAward RoundAward;

protected:
	UFUNCTION()
	void OnRep_Leaderboard();

	UFUNCTION()
	void OnRep_Award();
public:
	void CheckRoomLeader();
	void StartCountdown();
	void DecrementCountdown();
	void UpdateLeaderboard();
	void FinalizeAward(const TArray<TWeakObjectPtr<AEGPlayerController>>& Winners);
	void SetFinalResults(const TArray<TPair<TWeakObjectPtr<AEGPlayerController>, int32>>& Scores);

	UPROPERTY(ReplicatedUsing = OnRep_Award)
	TArray<FAward> RoundAwards;
	///여기까지 KMS
	
protected:
	UFUNCTION()
	void OnRep_RemainingCountdown();

	UFUNCTION()
	void OnRep_RemainingPlayTime();
};