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
	void OnEggLaid();

protected:
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* EggEnergyBar = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UImage* EggFX = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* BombEgg = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD|Egg")
	float UI_MaxEggEnergy = 100.f;

	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* TrickEgg = nullptr;

	// [추가] 기본 공격 쿨타임 바
	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* AttackBar = nullptr;

	// [추가] 대쉬 쿨타임 바
	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* DashBar = nullptr;

private:
	UPROPERTY() UAbilitySystemComponent* ASC = nullptr;

	FDelegateHandle StaminaChangedHandle;
	FDelegateHandle EggEnergyChangedHandle;

	bool bEggFXVisible = false;

	float BombCooldownRemaining = 0.f;
	float BombCooldownDuration  = 1.f;
	float TrickCooldownRemaining = 0.f;
	float TrickCooldownDuration  = 1.f;

	// [추가]
	float AttackCooldownRemaining = 0.f;
	float AttackCooldownDuration  = 1.f;

	// [추가]
	float DashCooldownRemaining = 0.f;
	float DashCooldownDuration  = 1.f;

	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnEggEnergyChanged(const FOnAttributeChangeData& Data);

	void RefreshStamina(float NewValue);
	void RefreshEggEnergy(float NewValue);

	void StartReadyFX();
	void StopReadyFX();

	void UpdateCooldownBarForTag(class UProgressBar* Bar,
		const struct FGameplayTag& CooldownTag,
		float& OutRemaining, float& OutDuration);

	void UpdateBombCooldown();
	void UpdateTrickCooldown();

	// [추가]
	void UpdateAttackCooldown();
	void UpdateDashCooldown();
};
