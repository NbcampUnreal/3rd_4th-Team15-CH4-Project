//EGSprintEffect.cpp

#include "AbilitySystem/GameplayEffect/EGSprintEffect.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UEGSprintEffect::UEGSprintEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FGameplayModifierInfo MoveSpeedModifier;
	MoveSpeedModifier.ModifierMagnitude = FScalableFloat(300.0f);
	MoveSpeedModifier.ModifierOp = EGameplayModOp::Additive;
	MoveSpeedModifier.Attribute = UEGCharacterAttributeSet::GetMoveSpeedAttribute();

	Modifiers.Add(MoveSpeedModifier);

	UTargetTagsGameplayEffectComponent* TargetTags = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTags"));
	if (TargetTags)
	{
		FInheritedTagContainer TagChanges = TargetTags->GetConfiguredTargetTagChanges();
		TagChanges.Added.AddTag(FGameplayTag::RequestGameplayTag(FName("Status.Sprint")));
		TargetTags->SetAndApplyTargetTagChanges(TagChanges);
        
		this->GEComponents.Add(TargetTags);
	}

	// JM : GameplayCue : Sprint 연결
	FGameplayEffectCue SprintCue;
	SprintCue.GameplayCueTags.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Status.Sprint")));
	GameplayCues.Add(SprintCue);
}
