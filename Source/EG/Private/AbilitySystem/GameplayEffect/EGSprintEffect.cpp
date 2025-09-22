//EGSprintEffect.cpp

#include "AbilitySystem/GameplayEffect/EGSprintEffect.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"

UEGSprintEffect::UEGSprintEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FGameplayModifierInfo MoveSpeedModifier;
	MoveSpeedModifier.ModifierMagnitude = FScalableFloat(300.0f);
	MoveSpeedModifier.ModifierOp = EGameplayModOp::Additive;
	MoveSpeedModifier.Attribute = UEGCharacterAttributeSet::GetMoveSpeedAttribute();

	Modifiers.Add(MoveSpeedModifier);
}
