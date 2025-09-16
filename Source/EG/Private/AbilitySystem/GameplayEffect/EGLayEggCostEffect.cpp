// EGLayEggCostEffect.cpp

#include "AbilitySystem/GameplayEffect/EGLayEggCostEffect.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGLayEggCostEffect::UEGLayEggCostEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo EggEnergyModifier;
	EggEnergyModifier.ModifierMagnitude = FScalableFloat(-100.0f);
	EggEnergyModifier.ModifierOp = EGameplayModOp::Additive;
	EggEnergyModifier.Attribute = UEGCharacterAttributeSet::GetEggEnergyAttribute();

	Modifiers.Add(EggEnergyModifier);
}
