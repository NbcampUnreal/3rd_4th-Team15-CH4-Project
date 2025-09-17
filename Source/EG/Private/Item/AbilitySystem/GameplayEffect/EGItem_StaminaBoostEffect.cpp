// EGItem_StaminaBoostEffect.cpp

#include "Item/AbilitySystem/GameplayEffect/EGItem_StaminaBoostEffect.h"

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGItem_StaminaBoostEffect::UEGItem_StaminaBoostEffect()
	:BoostDuration(10.0f)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(BoostDuration));

	//FGameplayModifierInfo StaminaModifier;
	//StaminaModifier.ModifierOp = EGameplayModOp::Override;
	//StaminaModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat())
	//StaminaModifier.Attribute = UEGCharacterAttributeSet::Get
}
