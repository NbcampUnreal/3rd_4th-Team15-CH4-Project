// EGPeckCooldownEffect.cpp

#include "AbilitySystem/GameplayEffect/EGPeckCooldownEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGPeckCooldownEffect::UEGPeckCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	DurationMagnitude = FScalableFloat(1.0f);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Cooldown.Peck")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);

		// 반드시 GameplayEffect에 등록
		// (중요: 이걸 안 하면 효과에 반영 안 됨)
		this->GEComponents.Add(TargetTags);
	}
	
}
