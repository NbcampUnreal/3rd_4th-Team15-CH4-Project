// EGItem_StaminaBoostEffect.cpp

#include "Item/AbilitySystem/GameplayEffect/EGItem_StaminaBoostEffect.h"

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGItem_StaminaBoostEffect::UEGItem_StaminaBoostEffect()
	:BoostDuration(10.0f)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(BoostDuration));

	// JM : Staking Policy 추가 (아이템 2번 연속으로 먹으면 시간 갱신되도록)
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;	

	//FGameplayModifierInfo StaminaModifier;
	//StaminaModifier.ModifierOp = EGameplayModOp::Override;
	//StaminaModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(100.f));
	//StaminaModifier.Attribute = UEGCharacterAttributeSet::GetStaminaAttribute();
	//Modifiers.Add(StaminaModifier);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
	TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Buff.StaminaBoost")));
	TargetTags->SetAndApplyTargetTagChanges(TagChanges);
	GEComponents.Add(TargetTags);

	// JM : GameplayCue_Actor : StaminaBoost 연결 (Local Client Only 재생되도록)
	FGameplayEffectCue StaminaBoostCue;
	StaminaBoostCue.GameplayCueTags.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Status.StaminaBoost.LocalOnly")));
	GameplayCues.Add(StaminaBoostCue);
}
