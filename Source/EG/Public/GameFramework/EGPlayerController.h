// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EGPlayerController.generated.h"

class ALevelSequenceActor;
class ULevelSequencePlayer;
class FOnMovieSceneSequencePlayerEvent;
class ULevelSequence;
class UAbilitySystemComponent;               // ★ 추가
class UWBP_HUD;    

class AEGHUD;                        // 김효영
/**
 * 
 */
UCLASS()
class EG_API AEGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Player")
	int32 PlayerIndex;
    
    //void SetPlayerIndex(int32 NewIndex);

// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
public:
	UFUNCTION(Server, Reliable)
	void ServerRequestLevelChange();

	UFUNCTION(Server, Reliable)
	void ServerRequestLevelRecordChange(const FString& MapName);

	void ToggleMouseCursor();

	UFUNCTION(Client, Reliable)
	void ClientRPCFadeInScreen();

	UFUNCTION(Client, Reliable)
	void ClientRPCFadeOutScreen();

#pragma endregion

// 채팅 (작성자 : 김효영)
#pragma region Chatting
public:

	UFUNCTION()
	void ActivateChatBox();

	UFUNCTION(Server, Reliable)
	void ServerSendChatMessage(const FString& Message);

	UFUNCTION(Client, Reliable)
	void ClientAddChatMessage(const FString& Message);


	UFUNCTION(Client, Reliable)
	void ClientGetOutWidget();
private:
	UPROPERTY()
	AEGHUD* EGHUD;
#pragma endregion

// 방장 기능(작성자: 김효영)
	
#pragma region ChiefPlayer 
public:
	UFUNCTION(Client, Reliable)
	void ShowChiefPlayerUI();
#pragma endregion

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// ===== 여기서부터 내가 추가한 부분 =====
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UWBP_HUD> HUDWidgetClass;     // ★ 전방선언 + SubclassOf 헤더로 해결

	UPROPERTY() UWBP_HUD* HUDRef = nullptr;

	FTimerHandle InitAscTimer;

	UFUNCTION() void TryInitHUD_ASC();
	UAbilitySystemComponent* FindASC() const;
	void CreateAndShowHUD();
	void BindHUDToASC();
	// ===== 여기까지 =====

// JM : 게임 시작/종료시 SFX 재생 
public:
	UFUNCTION(Client, Reliable)
	void ClientRPC_PlaySFXGameStart();

	UFUNCTION(Client, Reliable)
	void ClientRPC_PlaySFXGameOver();

// 시퀀스 재생 (작성자 : 김혁)
#pragma region Seqeuence

public:
	UFUNCTION(Client, Reliable)
	void ClientRPC_PlayEndingSequence(bool bIsWinner);

	UFUNCTION(Server, Reliable)
	void ServerRPC_NotifySequenceFinished();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowBlackScreen();

	UFUNCTION(Client, Reliable)
	void ClientRPC_HideBlackScreen();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sequence")
	TObjectPtr<ULevelSequence> CommonSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sequence")
	TObjectPtr<ULevelSequence> WinSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sequence")
	TObjectPtr<ULevelSequence> LoseSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Sequence|UI")
	TSubclassOf<UUserWidget> WinnerWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Sequence|UI")
	TSubclassOf<UUserWidget> LoserWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> EndingWidget;
	
	UPROPERTY()
	ULevelSequencePlayer* CurrentSequencePlayer;
	UPROPERTY()
	ALevelSequenceActor* CurrentSequenceActor;

	FTimerHandle SequenceTimerHandle;
	
	UFUNCTION()
	void OnCommonSequenceFinished();
	UFUNCTION()
	void OnFinalSequenceFinished();
	
private:
	bool bCachedIsWinner;
	bool bSequenceHandled = false;
	bool bCurrentSequenceIsFinal = false;

	UPROPERTY()
	ULevelSequence* FinalSequenceToPlay;
	
	void PlayLevelSequence(ULevelSequence* Sequence, bool bIsFinal, float OptionalDurationSeconds = -1.f);
	float GetSequenceDuration(ULevelSequence* Sequence) const;
	void HandleSequenceFallbackTimeout();
	
#pragma endregion 
};
