//EGLayTrickEggCooldownEffect.h

#include "AbilitySystem/GameplayEffect/EGLayTrickEggCooldownEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGLayTrickEggCooldownEffect::UEGLayTrickEggCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FScalableFloat(30.0f);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Cooldown.LayTrickEgg")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);

		this->GEComponents.Add(TargetTags);
	}
}
