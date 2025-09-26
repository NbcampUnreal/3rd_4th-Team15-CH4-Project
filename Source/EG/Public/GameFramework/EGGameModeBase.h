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
	void SetRoomLeader();
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	

	UFUNCTION(BlueprintCallable, Category="GameFlow")
	void GameStart(int32 UniqueID);
	UFUNCTION(BlueprintCallable, Category="GameFlow")
	void GameOver();
	void ClearStage();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	void InitializeSpawnPoint();

	FTimerHandle GameStartingTimerHandle;
	FTimerHandle GameEndTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<AEGAICharacter> AICharacter;
	
protected:
	bool bGameStarted = false;
	int32 LeaderNum = 0;

	UPROPERTY(VisibleAnywhere, Category="Players")
	TArray<TWeakObjectPtr<AEGPlayerController>> APlayingPlayerControllers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TMap<int32, AEGPlayerStart*> PlayerStartList;
	UPROPERTY(VisibleAnywhere, Category="Spawn")
	TArray<TWeakObjectPtr<AEGInGameSpawnPoints>> AInGameSpawnPoints;
};