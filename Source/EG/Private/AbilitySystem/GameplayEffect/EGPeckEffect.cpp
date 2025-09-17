// EGPeckEffect.cpp

#include "AbilitySystem/GameplayEffect/EGPeckEffect.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGPeckEffect::UEGPeckEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo EggEnergyModifier;
	EggEnergyModifier.ModifierMagnitude = FScalableFloat(10.0f);
	EggEnergyModifier.ModifierOp = EGameplayModOp::Additive;
	EggEnergyModifier.Attribute = UEGCharacterAttributeSet::GetEggEnergyAttribute();

	Modifiers.Add(EggEnergyModifier);
	
}
