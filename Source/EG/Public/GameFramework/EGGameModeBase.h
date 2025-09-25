#pragma once

#include "CoreMinimal.h"
#include "EGInGameSpawnPoints.h"
#include "GameFramework/GameModeBase.h"
#include "AI/EGAICharacter.h"
#include "GameFramework/EGPlayerController.h"

#include "EGGameModeBase.generated.h"

class AEGPlayerStart;


UCLASS()
class EG_API AEGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	 virtual void PreLogin(const FString& Options,
						  const FString& Address,
						  const FUniqueNetIdRepl& UniqueId,
						  FString& ErrorMessage) override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	void StartCount();
	void EndCount();

	UFUNCTION(BlueprintCallable, Category="GameFlow")
	void GameStart();
	UFUNCTION(BlueprintCallable, Category="GameFlow")
	void GameOver();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	void InitializeSpawnPoint();

// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(const FString& MapName);

#pragma endregion

// 채팅 (작성자 : 김효영)
#pragma region Chatting
	void SendChatMessage(const FString& Message);

#pragma endregion

	FTimerHandle GameStartingTimerHandle;
	FTimerHandle GameEndTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<AEGAICharacter> AICharacter;
	
protected:
	bool bGameStarted = false;

	UPROPERTY(VisibleAnywhere, Category="Players")
	TArray<TWeakObjectPtr<AEGPlayerController>> APlayingPlayerControllers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TMap<int32, AEGPlayerStart*> PlayerStartList;
	UPROPERTY(VisibleAnywhere, Category="Spawn")
	TArray<TWeakObjectPtr<AEGInGameSpawnPoints>> AInGameSpawnPoints;
	
	
};