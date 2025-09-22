#include "AbilitySystem/GameplayEffect/EGStaminaRegenEffect.h"

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGStaminaRegenEffect::UEGStaminaRegenEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	//DurationMagnitude = FScalableFloat(60.0f);
	Period = 1.0f;
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo StaminaRegenMod;
	StaminaRegenMod.Attribute = UEGCharacterAttributeSet::GetStaminaAttribute();
	StaminaRegenMod.ModifierOp = EGameplayModOp::Additive;
	StaminaRegenMod.ModifierMagnitude = FScalableFloat(10.0f);
	Modifiers.Add(StaminaRegenMod);
}
