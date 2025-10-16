#include "AbilitySystem/GameplayEffect/EGStaminaRegenEffect.h"

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGStaminaRegenEffect::UEGStaminaRegenEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 0.2f;
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo StaminaRegenMod;
	StaminaRegenMod.Attribute = UEGCharacterAttributeSet::GetStaminaAttribute();
	StaminaRegenMod.ModifierOp = EGameplayModOp::Additive;
	StaminaRegenMod.ModifierMagnitude = FScalableFloat(1.0f);
	Modifiers.Add(StaminaRegenMod);

	// 스태미나 재생 태그 붙이기 (작성작 : 김세훈)
	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Passive.StaminaRegen")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);
        
		this->GEComponents.Add(TargetTags);
	}
}
