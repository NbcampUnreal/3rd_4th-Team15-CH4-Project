#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "WBP_HUD.generated.h"

// 헤더는 가볍게: 전방선언만
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

	UPROPERTY(meta=(BindWidget))
	UProgressBar* StaminaBar = nullptr;

	UPROPERTY() UAbilitySystemComponent* ASC = nullptr;
	FDelegateHandle StaminaChangedHandle;

	void RefreshStamina(float NewValue);
	void OnStaminaChanged(const FOnAttributeChangeData& Data); // UFUNCTION 아님
};
