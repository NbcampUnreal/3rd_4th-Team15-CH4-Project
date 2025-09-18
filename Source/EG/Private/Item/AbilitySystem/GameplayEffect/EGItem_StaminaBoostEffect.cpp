// EGItem_StaminaBoostEffect.cpp

#include "Item/AbilitySystem/GameplayEffect/EGItem_StaminaBoostEffect.h"

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGItem_StaminaBoostEffect::UEGItem_StaminaBoostEffect()
	:BoostDuration(10.0f)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(BoostDuration));

	FGameplayModifierInfo StaminaModifier;
	StaminaModifier.ModifierOp = EGameplayModOp::Override;
	StaminaModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.f));
	StaminaModifier.Attribute = UEGCharacterAttributeSet::GetStaminaAttribute();
	Modifiers.Add(StaminaModifier);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
	TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Buff.StaminaBoost")));
	TargetTags->SetAndApplyTargetTagChanges(TagChanges);
	GEComponents.Add(TargetTags);
}
