//EGDashCostEffect.cpp

#include "AbilitySystem/GameplayEffect/EGDashCostEffect.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGDashCostEffect::UEGDashCostEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo StaminaModifier;
	StaminaModifier.ModifierMagnitude = FScalableFloat(-40.0f);
	StaminaModifier.ModifierOp = EGameplayModOp::Additive;
	StaminaModifier.Attribute = UEGCharacterAttributeSet::GetStaminaAttribute();

	Modifiers.Add(StaminaModifier);
}
