// EGPeckCooldownEffect.cpp

#include "AbilitySystem/GameplayEffect/EGPeckCooldownEffect.h"

UEGPeckCooldownEffect::UEGPeckCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FGameplayEffectModifierMagnitude(5.0f);
	
}
