// EGDelegateManager.h

#pragma once

#include "GameFramework/EGGameStateBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EGDelegateManager.generated.h"

class AEGItemBase;

#pragma region DECLARE_DELEGATE

//Gamstate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdated, int32, NewCountdown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayTimeUpdated, int32, NewPlayTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeaderboardUpdated, const TArray<FAward>&, NewLeaderboard);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAwardUpdated, const FAward&, NewAward);

#pragma endregion

/**
 * 
 */
UCLASS()
class EG_API UEGDelegateManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

#pragma region DELEGATE

	//GameState
	UPROPERTY(BlueprintAssignable, Category = "GameState|Events")
	FOnCountdownUpdated OnCountdownUpdated;

	UPROPERTY(BlueprintAssignable, Category = "GameState|Events")
	FOnPlayTimeUpdated OnPlayTimeUpdated;

	UPROPERTY(BlueprintAssignable, Category="GameState|Events")
	FOnLeaderboardUpdated OnLeaderboardUpdated;

	UPROPERTY(BlueprintAssignable, Category="GameState|Events")
	FOnAwardUpdated OnAwardUpdated;
	
#pragma endregion 

#pragma region BROADCAST
	
	void BroadcastPlayerState(AEGGameStateBase* GameState);

#pragma endregion 
};
