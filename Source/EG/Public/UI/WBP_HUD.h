#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "WBP_HUD.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;
class UImage; // [추가]

UCLASS()
class EG_API UWBP_HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="HUD")
	void InitWithASC(UAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category="HUD|Egg")
	void OnEggLaid(); // [추가] 알 낳기 성공 시 즉시 FX OFF

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UProgressBar* EggEnergyBar = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UImage* EggFX = nullptr; // [추가] 알 바 뒤의 FX 이미지(브러시에 MAT_UI_MaskRotate_Inst_01 지정)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD|Egg")
	float UI_MaxEggEnergy = 100.f;

private:
	UPROPERTY() UAbilitySystemComponent* ASC = nullptr;

	FDelegateHandle StaminaChangedHandle;
	FDelegateHandle EggEnergyChangedHandle;

	bool bEggReady = false; // [추가]

	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnEggEnergyChanged(const FOnAttributeChangeData& Data);

	void RefreshStamina(float NewValue);
	void RefreshEggEnergy(float NewValue);

	void StartReadyFX(); // [추가]
	void StopReadyFX();  // [추가]
};
