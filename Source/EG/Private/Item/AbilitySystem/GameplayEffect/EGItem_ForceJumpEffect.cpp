// EGItem_ForceJumpEffect.cpp

#include "Item/AbilitySystem/GameplayEffect/EGItem_ForceJumpEffect.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGItem_ForceJumpEffect::UEGItem_ForceJumpEffect()
	: EffectDuration(3.f)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(EffectDuration));

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
	TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Debuff.ForceJump")));
	TargetTags->SetAndApplyTargetTagChanges(TagChanges);
	GEComponents.Add(TargetTags);
}
