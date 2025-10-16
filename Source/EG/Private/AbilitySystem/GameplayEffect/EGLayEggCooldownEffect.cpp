// EGLayEggCooldownEffect.cpp

#include "AbilitySystem/GameplayEffect/EGLayEggCooldownEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGLayEggCooldownEffect::UEGLayEggCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FScalableFloat(1.0f);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Cooldown.LayEgg")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);

		this->GEComponents.Add(TargetTags);
	}
}
