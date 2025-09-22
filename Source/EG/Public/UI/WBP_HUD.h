#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "WBP_HUD.generated.h"

// 헤더는 가볍게: 전방선언
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS()
class EG_API UWBP_HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="HUD")
	void InitWithASC(UAbilitySystemComponent* InASC);

protected:
	virtual void NativeDestruct() override;

	// === 기존 스태미나 바 ===
	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar = nullptr;

	// === 새로 추가: 알(EggEnergy) 바 ===
	// 위젯이 아직 없어도 크래시 안 나게 Optional 권장
	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* EggEnergyBar = nullptr;

	// UI 분모(팀 코드 수정 없이 0~100로 정규화)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD|Egg")
	float UI_MaxEggEnergy = 100.f;

	// 내부
	UPROPERTY() UAbilitySystemComponent* ASC = nullptr;

	// 델리게이트 핸들
	FDelegateHandle StaminaChangedHandle;
	FDelegateHandle EggEnergyChangedHandle;

	// 콜백 & 반영
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnEggEnergyChanged(const FOnAttributeChangeData& Data);

	void RefreshStamina(float NewValue);
	void RefreshEggEnergy(float NewValue);
};

// 한국인
