#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "WBP_HUD.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;
class UImage;

UCLASS()
class EG_API UWBP_HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="HUD")
	void InitWithASC(UAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category="HUD|Egg")
	void OnEggLaid(); // [추가]

protected:
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // [추가]

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* EggEnergyBar = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UImage* EggFX = nullptr; // [추가]

	// [수정] 이름 통일: BombEgg (UMG의 ProgressBar 변수명도 동일해야 함)
	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* BombEgg = nullptr; // 폭탄알 쿨타임 바

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD|Egg")
	float UI_MaxEggEnergy = 100.f;

	// [추가] 함정알(TrickEgg) 쿨타임 바
	UPROPERTY(meta=(BindWidgetOptional)) // WBP에 존재하면 자동 바인딩
	UProgressBar* TrickEgg = nullptr;   // [추가]

private:
	UPROPERTY() UAbilitySystemComponent* ASC = nullptr;

	FDelegateHandle StaminaChangedHandle;
	FDelegateHandle EggEnergyChangedHandle;

	bool bEggFXVisible = false; // [수정]

	// 표시용 캐시
	float BombCooldownRemaining = 0.f; // [추가]
	float BombCooldownDuration  = 1.f; // [추가]

	// [추가] 함정알 캐시
	float TrickCooldownRemaining = 0.f; // [추가]
	float TrickCooldownDuration  = 1.f; // [추가]

	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnEggEnergyChanged(const FOnAttributeChangeData& Data);

	void RefreshStamina(float NewValue);
	void RefreshEggEnergy(float NewValue);

	void StartReadyFX(); // [추가]
	void StopReadyFX();  // [추가]

	// [추가] 공통 쿨타임 업데이트(태그 → 바 갱신)
	void UpdateCooldownBarForTag(class UProgressBar* Bar,
		const struct FGameplayTag& CooldownTag,
		float& OutRemaining, float& OutDuration);

	// [추가] 편의 래퍼
	void UpdateBombCooldown();
	void UpdateTrickCooldown();
};
