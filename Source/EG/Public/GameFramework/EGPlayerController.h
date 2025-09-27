// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EGPlayerController.generated.h"

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
    
    void SetPlayerIndex(int32 NewIndex);

// 레벨 변경 (작성자 : 김효영)
#pragma region LevelChange
	UFUNCTION(Server, Reliable)
	void ServerRequestLevelChange();

	UFUNCTION(Server, Reliable)
	void ServerRequestLevelRecordChange(const FString& MapName);

#pragma endregion

// 채팅 (작성자 : 김효영)
#pragma region Chatting
	

	UFUNCTION()
	void ActivateChatBox();

	UFUNCTION(Server, Reliable)
	void ServerSendChatMessage(const FString& Message);

	UFUNCTION(Client, Reliable)
	void ClientAddChatMessage(const FString& Message);

private:
	UPROPERTY()
	AEGHUD* EGHUD;
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
	
};
