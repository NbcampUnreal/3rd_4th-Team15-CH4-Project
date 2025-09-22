//EGAttackCooldownEffect.cpp

#include "AbilitySystem/GameplayEffect/EGAttackCooldownEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGAttackCooldownEffect::UEGAttackCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	
	DurationMagnitude = FScalableFloat(7.0f);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Cooldown.Attack")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);

		this->GEComponents.Add(TargetTags);
	}
}
