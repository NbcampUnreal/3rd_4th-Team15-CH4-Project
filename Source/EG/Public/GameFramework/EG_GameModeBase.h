#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EG_GameModeBase.generated.h"

class AEG_PlayerStart;
class AEG_PlayerController;
class AEG_GameStateBase;

UCLASS()
class YOURGAME_API AEG_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Game")
	void GameStart();
	
protected:
	UFUNCTION(BlueprintCallable, Category="Spawn")
	void InitializeSpawnPoint();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	bool IsSpawnPointAvailable(const AActor* Start) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	TArray<AEG_PlayerStart*> SpawnPoints;

	bool bGameStarted = false;
	
	UPROPERTY(VisibleAnywhere, Category="Players")
	TArray<TWeakObjectPtr<AEG_PlayerController>> APlayingPlayerControllers;
};