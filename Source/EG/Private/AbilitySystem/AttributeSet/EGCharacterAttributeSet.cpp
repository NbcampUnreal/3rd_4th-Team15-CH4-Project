// EGCharacterAttributeSet.cpp

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

UEGCharacterAttributeSet::UEGCharacterAttributeSet()
{
	InitEggEnergy(100.0f);
}

void UEGCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEGCharacterAttributeSet, EggEnergy, COND_None, REPNOTIFY_Always);
}

void UEGCharacterAttributeSet::OnRep_EggEnergy(const FGameplayAttributeData& OldEggEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEGCharacterAttributeSet, EggEnergy, OldEggEnergy)
}
