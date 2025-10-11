#pragma once
#include "CoreMinimal.h"
#include "EGInGameSpawnPoints.h"
#include "EGPlayerState.h"
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
	void HandleSeamlessTravelPlayer(AController* C);
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

// JM : 서버이동 (게임 끝나고 SFX 재생하는데, 월드 파괴시 예외발생으로 5초 뒤 이동)
	void TravelToLevel();	
	void FadeInScreen();
	void FadeOutScreen();

#pragma endregion


	FTimerHandle GameStartingTimerHandle;
	FTimerHandle GameEndTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<AEGAICharacter> AICharacter;
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Players")
	TArray<TWeakObjectPtr<AEGPlayerState>> APlayingPlayerState;
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
