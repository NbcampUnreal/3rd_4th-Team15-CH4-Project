// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/DelegateCombinations.h"
#include "EGGameStateBase.generated.h"

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

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdated, int32, NewCountdown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayTimeUpdated, int32, NewPlayTime);

UCLASS()
class EG_API AEGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 AlivePlayerControllerCount = 0;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	EMatchState MatchState = EMatchState::Waiting;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingCountdown, VisibleAnywhere, BlueprintReadOnly)
	int32 RemainingCountdown = 100;

	 UPROPERTY(ReplicatedUsing = OnRep_RemainingPlayTime, VisibleAnywhere, BlueprintReadOnly)
	 int32 RemainingPlayTime = 100;

	 UPROPERTY(BlueprintAssignable, Category = "GameState|Events")
	 FOnCountdownUpdated OnCountdownUpdated;

	 UPROPERTY(BlueprintAssignable, Category = "GameState|Events")
	 FOnPlayTimeUpdated OnPlayTimeUpdated;

protected:
	UFUNCTION()
	void OnRep_RemainingCountdown();

	UFUNCTION()
	void OnRep_RemainingPlayTime();

};
