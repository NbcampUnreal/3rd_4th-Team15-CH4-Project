// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/EGPlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "EGLobbyGameModeBase.generated.h"


class UEggPoolDataAsset;
class AEGPlayerController;
class AEGPlayerStart;

/**
 * 
 */
UCLASS()
class EG_API AEGLobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AEGLobbyGameModeBase();
#pragma region Chatting
	void SendChatMessage(const FString& Message);

#pragma endregion
	 int32 CurrentPlayerIndex = 0;

	virtual void PreLogin(const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	void InitializeSpawnPoint();

// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
	void LevelChange();

	void ShowScreen();
	//void HideScreen();

private:
	bool bLevelChanging = false;

#pragma endregion


	FTimerHandle GameStartingTimerHandle;
	FTimerHandle GameEndTimerHandle;

protected:
	bool bChiefPlayer = false;
	int32 LeaderNum = 0;


	UPROPERTY(VisibleAnywhere, Category = "Players")
	TArray<TWeakObjectPtr<AEGPlayerState>> APlayingPlayerStates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TMap<int32, AEGPlayerStart*> PlayerStartList;

// JM : 오브젝트 풀링
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EggPool")
	UEggPoolDataAsset* EggPoolDataAsset;
};

#pragma endregion
