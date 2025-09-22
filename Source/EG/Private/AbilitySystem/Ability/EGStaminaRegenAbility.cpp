#include "AbilitySystem/Ability/EGStaminaRegenAbility.h"

#include "AbilitySystem/GameplayEffect/EGStaminaRegenEffect.h"
#include "AbilitySystemComponent.h"

UEGStaminaRegenAbility::UEGStaminaRegenAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//StaminaRegenEffectClass = UEGStaminaRegenAbility::StaticClass();
	StaminaRegenEffectClass = UEGStaminaRegenEffect::StaticClass();
}

void UEGStaminaRegenAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (StaminaRegenEffectClass && ActorInfo->AbilitySystemComponent.IsValid())
    {
        FGameplayEffectContextHandle ContextHandle =
            ActorInfo->AbilitySystemComponent->MakeEffectContext();
        ContextHandle.AddSourceObject(this);

        FGameplayEffectSpecHandle SpecHandle =
            ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
                StaminaRegenEffectClass, 1, ContextHandle);

        if (SpecHandle.IsValid())
        {
            FActiveGameplayEffectHandle ActiveHandle =
                ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
                    *SpecHandle.Data.Get());

            if (ActiveHandle.WasSuccessfullyApplied())
            {
                UE_LOG(LogTemp, Log, TEXT("Staminaregen..."));
            }
        }
    }

    //EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UEGStaminaRegenAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    UE_LOG(LogTemp, Log, TEXT("StaminaRegen ability ended"));
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
