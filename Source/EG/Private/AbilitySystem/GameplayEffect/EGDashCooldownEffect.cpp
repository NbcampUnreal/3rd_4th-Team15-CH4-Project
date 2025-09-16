//EGDashCooldownEffect.cpp

#include "AbilitySystem/GameplayEffect/EGDashCooldownEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGDashCooldownEffect::UEGDashCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FScalableFloat(5.0f);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Cooldown.Dash")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);

		this->GEComponents.Add(TargetTags);
	}
}
