#include "AbilitySystem/GameplayEffect/EGStaminaRegenEffect.h"

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGStaminaRegenEffect::UEGStaminaRegenEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 0.2f;
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo StaminaRegenMod;
	StaminaRegenMod.Attribute = UEGCharacterAttributeSet::GetStaminaAttribute();
	StaminaRegenMod.ModifierOp = EGameplayModOp::Additive;
	StaminaRegenMod.ModifierMagnitude = FScalableFloat(1.0f);
	Modifiers.Add(StaminaRegenMod);
}
