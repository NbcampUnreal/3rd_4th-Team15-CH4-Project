#include "UI/WBP_HUD.h"                                  // <- 경로 명시
#include "AbilitySystemComponent.h"                       // FOnAttributeChangeData, ASC
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h" // 트리 그대로!

void UWBP_HUD::InitWithASC(UAbilitySystemComponent* InASC)
{
	if (ASC && StaminaChangedHandle.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(
			UEGCharacterAttributeSet::GetStaminaAttribute())
			.Remove(StaminaChangedHandle);
	}

	ASC = InASC;
	if (!ASC) return;

	const float Cur =
		ASC->GetNumericAttribute(UEGCharacterAttributeSet::GetStaminaAttribute());
	RefreshStamina(Cur);

	StaminaChangedHandle =
		ASC->GetGameplayAttributeValueChangeDelegate(
			UEGCharacterAttributeSet::GetStaminaAttribute())
		.AddUObject(this, &UWBP_HUD::OnStaminaChanged);
}

void UWBP_HUD::NativeDestruct()
{
	if (ASC && StaminaChangedHandle.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(
			UEGCharacterAttributeSet::GetStaminaAttribute())
			.Remove(StaminaChangedHandle);
	}
	StaminaChangedHandle.Reset();
	ASC = nullptr;
	Super::NativeDestruct();
}

void UWBP_HUD::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	RefreshStamina(Data.NewValue);
}

void UWBP_HUD::RefreshStamina(float NewValue)
{
	if (!StaminaBar) return;
	StaminaBar->SetPercent(FMath::Clamp(NewValue / 100.f, 0.f, 1.f)); // 분모 100 고정
}
