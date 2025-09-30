// EGGameInstance.h (작성자 : JJM)
// Voice chat 관련 기능을 Blueprint로 작성함
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EGGameInstance.generated.h"

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

private:
	UPROPERTY()
	FString CurrentLevelName = "Level_1";
#pragma endregion
};
