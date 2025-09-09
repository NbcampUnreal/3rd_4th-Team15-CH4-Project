// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EG_GameStateBase.generated.h"

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
UCLASS()
class EG_API AEG_GameStateBase : public AGameStateBase
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

	UFUNCTION()
	void OnRep_RemainingCountdown();

	UFUNCTION()
	void OnRep_RemainingPlayTime();

};
