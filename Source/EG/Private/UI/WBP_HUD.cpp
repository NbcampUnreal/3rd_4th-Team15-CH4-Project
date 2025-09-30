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
	if (BombEgg)  BombEgg->SetPercent(1.f);
	if (TrickEgg) TrickEgg->SetPercent(1.f); // [추가]

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
	UpdateBombCooldown();   // [추가]
	UpdateTrickCooldown();  // [추가]
}

// [추가] 공통 쿨타임 처리: 태그가 활성화되어 있으면 0→1로 차오르게, 없으면 1.0 유지
void UWBP_HUD::UpdateCooldownBarForTag(UProgressBar* Bar,
	const FGameplayTag& CooldownTag,
	float& OutRemaining, float& OutDuration)
{
	if (!ASC || !Bar) return;

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
		OutRemaining = MaxRemaining;
		OutDuration  = MatchedDuration;

		const float Pct = 1.f - (MaxRemaining / MatchedDuration); // 0→1로 차오름
		Bar->SetPercent(Pct);
	}
	else
	{
		Bar->SetPercent(1.f); // 쿨 없음 → 사용 가능
	}
}

void UWBP_HUD::UpdateBombCooldown()
{
	// 기존 폭탄알 태그
	static const FGameplayTag BombCooldownTag =
		FGameplayTag::RequestGameplayTag(TEXT("Ability.Cooldown.LayBombEgg"));

	UpdateCooldownBarForTag(BombEgg, BombCooldownTag, BombCooldownRemaining, BombCooldownDuration); // [추가]
}

void UWBP_HUD::UpdateTrickCooldown()
{
	// [추가] 함정알(팀원 Ability) 태그 — 프로젝트 GameplayTags에 등록되어 있어야 함
	static const FGameplayTag TrickCooldownTag =
		FGameplayTag::RequestGameplayTag(TEXT("Ability.Cooldown.LayTrickEgg"));

	UpdateCooldownBarForTag(TrickEgg, TrickCooldownTag, TrickCooldownRemaining, TrickCooldownDuration); // [추가]
}
