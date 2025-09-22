#include "UI/WBP_HUD.h"                                  // <- 경로 명시
#include "AbilitySystemComponent.h"                       // FOnAttributeChangeData, ASC
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h" // 트리 그대로!

void UWBP_HUD::InitWithASC(UAbilitySystemComponent* InASC)
{
    // 기존 바인딩 해제
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

    // 초기값 반영
    {
        const float CurSta =
            ASC->GetNumericAttribute(UEGCharacterAttributeSet::GetStaminaAttribute());
        RefreshStamina(CurSta);

        const float CurEgg =
            ASC->GetNumericAttribute(UEGCharacterAttributeSet::GetEggEnergyAttribute());
        RefreshEggEnergy(CurEgg);
    }

    // 델리게이트 바인딩
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
    StaminaBar->SetPercent(FMath::Clamp(NewValue / 100.f, 0.f, 1.f)); // 분모 100 고정
}

void UWBP_HUD::RefreshEggEnergy(float NewValue)
{
    if (!EggEnergyBar) return;

    const float Den = FMath::Max(1.f, UI_MaxEggEnergy);   // 0 나눗셈 방지
    const float Pct = FMath::Clamp(NewValue / Den, 0.f, 1.f);
    EggEnergyBar->SetPercent(Pct);
}

// 한국인
