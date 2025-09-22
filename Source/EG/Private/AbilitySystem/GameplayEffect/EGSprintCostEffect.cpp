//EGSprintCostEffect.cpp

#include "AbilitySystem/GameplayEffect/EGSprintCostEffect.h"

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGSprintCostEffect::UEGSprintCostEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 0.1f;
	bExecutePeriodicEffectOnApplication = false;
	
	FGameplayModifierInfo StaminaModifier;
	StaminaModifier.ModifierMagnitude = FScalableFloat(-2.0f);
	StaminaModifier.ModifierOp = EGameplayModOp::Additive;
	StaminaModifier.Attribute = UEGCharacterAttributeSet::GetStaminaAttribute();
	
	Modifiers.Add(StaminaModifier);
}
