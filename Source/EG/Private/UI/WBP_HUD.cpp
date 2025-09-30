#include "UI/WBP_HUD.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"

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

	StopReadyFX();
	bEggFXVisible = false;

	// [수정] 시작 시 스킬 ‘사용 가능’ 느낌으로 100% 표시
	if (BombEgg) BombEgg->SetPercent(1.f);

	if (!ASC) return;

	{
		const float CurSta = ASC->GetNumericAttribute(UEGCharacterAttributeSet::GetStaminaAttribute());
		RefreshStamina(CurSta);

		const float CurEgg = ASC->GetNumericAttribute(UEGCharacterAttributeSet::GetEggEnergyAttribute());
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

	StopReadyFX();
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

	const bool bNowShowFX = (Pct >= 0.7f);

	if (bNowShowFX && !bEggFXVisible)
	{
		bEggFXVisible = true;
		StartReadyFX();
	}
	else if (!bNowShowFX && bEggFXVisible)
	{
		bEggFXVisible = false;
		StopReadyFX();
	}
}

void UWBP_HUD::OnEggLaid()
{
	if (bEggFXVisible)
	{
		bEggFXVisible = false;
		StopReadyFX();
	}
}

void UWBP_HUD::StartReadyFX()
{
	if (EggFX)
	{
		EggFX->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UWBP_HUD::StopReadyFX()
{
	if (EggFX)
	{
		EggFX->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWBP_HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateBombCooldown();
}

void UWBP_HUD::UpdateBombCooldown()
{
	if (!ASC || !BombEgg) return;

	static const FGameplayTag CooldownTag =
		FGameplayTag::RequestGameplayTag(TEXT("Ability.Cooldown.LayBombEgg"));

	FGameplayTagContainer OwningTags;
	OwningTags.AddTag(CooldownTag);
	const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(OwningTags);

	TArray<FActiveGameplayEffectHandle> Handles = ASC->GetActiveEffects(Query);

	float MaxRemaining = -1.f;
	float MatchedDuration = 0.f;

	const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;

	for (const FActiveGameplayEffectHandle& H : Handles)
	{
		const FActiveGameplayEffect* AGE = ASC->GetActiveGameplayEffect(H);
		if (!AGE) continue;

		const float Rem = AGE->GetTimeRemaining(Now);
		const float Dur = AGE->Spec.GetDuration();

		if (Rem >= 0.f && Dur > 0.f && Rem > MaxRemaining)
		{
			MaxRemaining   = Rem;
			MatchedDuration = Dur;
		}
	}

	if (MaxRemaining >= 0.f && MatchedDuration > 0.f)
	{
		BombCooldownRemaining = MaxRemaining;
		BombCooldownDuration  = MatchedDuration;

		// 쿨타임 시작 시 0 → 끝날 때 1 로 ‘차오르는’ 표시
		const float Pct = 1.f - (MaxRemaining / MatchedDuration);
		BombEgg->SetPercent(Pct);
	}
	else
	{
		// [수정] 쿨타임이 없으면 ‘사용 가능’ 상태이므로 100%로 표시
		BombEgg->SetPercent(1.f);
	}
}
