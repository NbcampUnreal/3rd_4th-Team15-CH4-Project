//EGStunEffect.cpp

#include "AbilitySystem/GameplayEffect/EGStunEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGStunEffect::UEGStunEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(5.0f);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Status.Stunned")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);
        
		this->GEComponents.Add(TargetTags);
	}

}
