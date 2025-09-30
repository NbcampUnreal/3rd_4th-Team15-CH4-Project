// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EGLobbyGameModeBase.generated.h"


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

#pragma region Chatting
	void SendChatMessage(const FString& Message);

#pragma endregion
	 int32 CurrentPlayerIndex = 0;

	virtual void PreLogin(const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	void SetRoomLeader();
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;


	/*UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void GameStart(int32 UniqueID);*/

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	void InitializeSpawnPoint();

	FTimerHandle GameStartingTimerHandle;
	FTimerHandle GameEndTimerHandle;

protected:
	bool bGameStarted = false;
	int32 LeaderNum = 0;


	UPROPERTY(VisibleAnywhere, Category = "Players")
	TArray<TWeakObjectPtr<AEGPlayerController>> APlayingPlayerControllers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TMap<int32, AEGPlayerStart*> PlayerStartList;
};
