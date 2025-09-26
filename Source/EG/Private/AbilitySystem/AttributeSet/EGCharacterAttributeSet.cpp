// EGCharacterAttributeSet.cpp

#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UEGCharacterAttributeSet::UEGCharacterAttributeSet()
{
	InitMaxEggEnergy(100.0f);
	InitEggEnergy(0.0f);
	InitMaxStamina(100.0f);
	InitStamina(100.0f);
	InitMoveSpeed(400.f);
}

void UEGCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEGCharacterAttributeSet, MaxEggEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEGCharacterAttributeSet, EggEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEGCharacterAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEGCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEGCharacterAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UEGCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetStaminaAttribute())
	{
		// KH : For Stamina Boost Item 
		if (const UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
		{
			if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Buff.StaminaBoost"))))
			{
				NewValue = GetMaxStamina();
			}
		}

		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetEggEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxEggEnergy());
	}
}

void UEGCharacterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetEggEnergyAttribute())
	{
		SetEggEnergy(FMath::Clamp(GetEggEnergy(), 0.0f, GetMaxEggEnergy()));
	}
}

void UEGCharacterAttributeSet::OnRep_MaxEggEnergy(const FGameplayAttributeData& OldMaxEggEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEGCharacterAttributeSet, MaxEggEnergy, OldMaxEggEnergy);   
}

void UEGCharacterAttributeSet::OnRep_EggEnergy(const FGameplayAttributeData& OldEggEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEGCharacterAttributeSet, EggEnergy, OldEggEnergy);
}

void UEGCharacterAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEGCharacterAttributeSet, MaxStamina, OldMaxStamina);
}

void UEGCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEGCharacterAttributeSet, Stamina, OldStamina);
}

void UEGCharacterAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEGCharacterAttributeSet, MoveSpeed, OldMoveSpeed);
}
