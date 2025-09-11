#pragma once

#include "CoreMinimal.h"
#include "EG_InGameSpawnPoints.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/EG_PlayerController.h"
#include "EG_GameModeBase.generated.h"

class AEG_PlayerStart;


UCLASS()
class EG_API AEG_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Game")
	void GameStart();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	void InitializeSpawnPoint();

	FTimerHandle MyTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TMap<int32, AEG_PlayerStart*> PlayerStartList;
	UPROPERTY(VisibleAnywhere, Category="Spawn")
	TArray<TWeakObjectPtr<AEG_InGameSpawnPoints>> AInGameSpawnPoints;

protected:
	bool bGameStarted = false;
	
	UPROPERTY(VisibleAnywhere, Category="Players")
	TArray<TWeakObjectPtr<AEG_PlayerController>> APlayingPlayerControllers;

	
	
};