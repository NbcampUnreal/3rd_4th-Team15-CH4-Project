#include "UI/WBP_HUD.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h" // [추가]

void UWBP_HUD::InitWithASC(UAbilitySystemComponent* InASC)
{
	if (ASC)
	{
		if (StaminaChangedHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(
				UEGCharacterAttributeSet::GetStaminaAttribute())
				.Remove(StaminaChangedHandle);
		}
		if (EggEnergyChangedHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(
				UEGCharacterAttributeSet::GetEggEnergyAttribute())
				.Remove(EggEnergyChangedHandle);
		}
		StaminaChangedHandle.Reset();
		EggEnergyChangedHandle.Reset();
	}

	ASC = InASC;
	if (!ASC) return;

	{
		const float CurSta =
			ASC->GetNumericAttribute(UEGCharacterAttributeSet::GetStaminaAttribute());
		RefreshStamina(CurSta);

		const float CurEgg =
			ASC->GetNumericAttribute(UEGCharacterAttributeSet::GetEggEnergyAttribute());
		RefreshEggEnergy(CurEgg);
	}

	StaminaChangedHandle =
		ASC->GetGameplayAttributeValueChangeDelegate(
			UEGCharacterAttributeSet::GetStaminaAttribute())
		.AddUObject(this, &UWBP_HUD::OnStaminaChanged);

	EggEnergyChangedHandle =
		ASC->GetGameplayAttributeValueChangeDelegate(
			UEGCharacterAttributeSet::GetEggEnergyAttribute())
		.AddUObject(this, &UWBP_HUD::OnEggEnergyChanged);
}

void UWBP_HUD::NativeDestruct()
{
	if (ASC)
	{
		if (StaminaChangedHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(
				UEGCharacterAttributeSet::GetStaminaAttribute())
				.Remove(StaminaChangedHandle);
		}
		if (EggEnergyChangedHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(
				UEGCharacterAttributeSet::GetEggEnergyAttribute())
				.Remove(EggEnergyChangedHandle);
		}
	}
	StaminaChangedHandle.Reset();
	EggEnergyChangedHandle.Reset();

	StopReadyFX(); // [추가]

	ASC = nullptr;
	Super::NativeDestruct();
}

void UWBP_HUD::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	RefreshStamina(Data.NewValue);
}

void UWBP_HUD::OnEggEnergyChanged(const FOnAttributeChangeData& Data)
{
	RefreshEggEnergy(Data.NewValue);
}

void UWBP_HUD::RefreshStamina(float NewValue)
{
	if (!StaminaBar) return;
	StaminaBar->SetPercent(FMath::Clamp(NewValue / 100.f, 0.f, 1.f));
}

void UWBP_HUD::RefreshEggEnergy(float NewValue)
{
	if (!EggEnergyBar) return;

	const float Den = FMath::Max(1.f, UI_MaxEggEnergy);
	const float Pct = FMath::Clamp(NewValue / Den, 0.f, 1.f);
	EggEnergyBar->SetPercent(Pct);

	const bool bNowReady = (Pct >= 1.f - KINDA_SMALL_NUMBER);
	if (bNowReady && !bEggReady) // [수정] Ready 진입 시 FX ON
	{
		bEggReady = true;
		StartReadyFX();
	}
	else if (!bNowReady && bEggReady) // [수정] Ready 해제 시 FX OFF
	{
		bEggReady = false;
		StopReadyFX();
	}
}

void UWBP_HUD::OnEggLaid() // [추가]
{
	if (bEggReady)
	{
		bEggReady = false;
		StopReadyFX();
	}
}

void UWBP_HUD::StartReadyFX() // [추가]
{
	if (EggFX)
	{
		EggFX->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UWBP_HUD::StopReadyFX() // [추가]
{
	if (EggFX)
	{
		EggFX->SetVisibility(ESlateVisibility::Collapsed);
	}
}
