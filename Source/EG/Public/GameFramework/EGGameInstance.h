// EGGameInstance.h (작성자 : JJM)
// Voice chat 관련 기능을 Blueprint로 작성함
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EGGameInstance.generated.h"

USTRUCT()
struct FFinalResult
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 PlayerId = 0;

	UPROPERTY()
	bool bIsWinner = false;
};

/**
 * 
 */
UCLASS()
class EG_API UEGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 작성자: 김효영

#pragma region Chatting
	void SendChatMessage(const FString& Message);

#pragma endregion

#pragma region LevelChange
	
	UFUNCTION(BlueprintCallable)
	void RecordLevel(const FString& MapName);

	UFUNCTION(BlueprintCallable)
	void ChangeLevel();

	void FadeInScreen();
	void FadeOutScreen();
	UFUNCTION(BlueprintCallable)
	void FadeOutScreenRemove();

	UPROPERTY()
	UUserWidget* FadeInScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> FadeInScreenClass;

	UPROPERTY()
	UUserWidget* FadeOutcreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> FadeOutScreenClass;

private:
	UPROPERTY()
	FString CurrentLevelName = "Level_1";

#pragma endregion

public:
	int32 GetPlayerIndex()
	{
		return PlayerIndex;
	}

	void PlayerIndexReset()
	{
		if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		{
			PlayerIndex = 0;
		}
	}

	void SetPlayerIndex(int32 Index)
	{
		if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		{
			PlayerIndex += Index;
		}
	}

	UPROPERTY();
	int32 PlayerIndex = 0;

// 작성자: 김혁
#pragma region MatchResult

public:
	UPROPERTY()
	TArray<FFinalResult> FinalResults;

	void SetFinalResults(const TArray<FFinalResult>& InResults);
	TArray<FFinalResult>& GetFinalResults();

#pragma endregion 
};
