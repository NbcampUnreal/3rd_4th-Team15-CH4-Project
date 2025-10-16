//EGResetEggEnergyEffect.cpp

#include "AbilitySystem/GameplayEffect/EGResetEggEnergyEffect.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGResetEggEnergyEffect::UEGResetEggEnergyEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo EggEnergyModifier;
	EggEnergyModifier.ModifierMagnitude = FScalableFloat(0.0f);
	EggEnergyModifier.ModifierOp = EGameplayModOp::Override;
	EggEnergyModifier.Attribute = UEGCharacterAttributeSet::GetEggEnergyAttribute();
	
	Modifiers.Add(EggEnergyModifier);
}
