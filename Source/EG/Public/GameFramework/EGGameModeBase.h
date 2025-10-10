#pragma once
#include "CoreMinimal.h"
#include "EGInGameSpawnPoints.h"
#include "GameFramework/GameModeBase.h"
#include "AI/EGAICharacter.h"
#include "Character/Egg/EggPoolDataAsset.h"
#include "GameFramework/EGPlayerController.h"
#include "EGGameModeBase.generated.h"
class AEGPlayerStart;
UCLASS()
class EG_API AEGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AEGGameModeBase();
	int32 CurrentPlayerIndex = 0;
	virtual void PreLogin(const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category="GameFlow")
	void GameStart();
	UFUNCTION(BlueprintCallable, Category="GameFlow")
	void GameOver();
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	void InitializeSpawnPoint();

// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
	void ShowScreen();
	void HideScreen();

#pragma endregion


	FTimerHandle GameStartingTimerHandle;
	FTimerHandle GameEndTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<AEGAICharacter> AICharacter;
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Players")
	TArray<TWeakObjectPtr<AEGPlayerController>> APlayingPlayerControllers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TMap<int32, AEGPlayerStart*> PlayerStartList;
	UPROPERTY(VisibleAnywhere, Category="Spawn")
	TArray<TWeakObjectPtr<AEGInGameSpawnPoints>> AInGameSpawnPoints;
	
private:
	int32 playerCount = 0;

// JM : 오브젝트 풀링
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EggPool")
	UEggPoolDataAsset* EggPoolDataAsset;
};
