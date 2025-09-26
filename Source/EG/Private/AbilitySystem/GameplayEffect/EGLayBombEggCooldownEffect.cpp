//EGLayBombEggCooldownEffect.cpp

#include "AbilitySystem/GameplayEffect/EGLayBombEggCooldownEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGLayBombEggCooldownEffect::UEGLayBombEggCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FScalableFloat(40.0f);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Cooldown.LayBombEgg")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);

		this->GEComponents.Add(TargetTags);
	}
}
