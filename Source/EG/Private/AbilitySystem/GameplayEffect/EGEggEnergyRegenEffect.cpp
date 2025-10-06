#include "AbilitySystem/GameplayEffect/EGEggEnergyRegenEffect.h"

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGEggEnergyRegenEffect::UEGEggEnergyRegenEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 1.0f;
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo EggEnergyRegenMod;
	EggEnergyRegenMod.Attribute = UEGCharacterAttributeSet::GetEggEnergyAttribute();
	EggEnergyRegenMod.ModifierOp = EGameplayModOp::Additive;
	EggEnergyRegenMod.ModifierMagnitude = FScalableFloat(2.0f);
	Modifiers.Add(EggEnergyRegenMod);
}
