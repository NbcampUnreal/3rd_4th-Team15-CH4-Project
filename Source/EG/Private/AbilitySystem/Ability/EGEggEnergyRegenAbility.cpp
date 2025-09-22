#include "AbilitySystem/Ability/EGEggEnergyRegenAbility.h"

#include "AbilitySystem/GameplayEffect/EGEggEnergyRegenEffect.h"
#include "AbilitySystemComponent.h"

UEGEggEnergyRegenAbility::UEGEggEnergyRegenAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	EggEnergyRegenEffectClass = UEGEggEnergyRegenEffect::StaticClass();
}

void UEGEggEnergyRegenAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (EggEnergyRegenEffectClass && ActorInfo->AbilitySystemComponent.IsValid())
    {
        FGameplayEffectContextHandle ContextHandle =
            ActorInfo->AbilitySystemComponent->MakeEffectContext();
        ContextHandle.AddSourceObject(this);

        FGameplayEffectSpecHandle SpecHandle =
            ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
                EggEnergyRegenEffectClass, 1, ContextHandle);

        if (SpecHandle.IsValid())
        {
            FActiveGameplayEffectHandle ActiveHandle =
                ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
                    *SpecHandle.Data.Get());

            if (ActiveHandle.WasSuccessfullyApplied())
            {
                UE_LOG(LogTemp, Log, TEXT("EggEnergy..."));
            }
        }
    }

    //EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UEGEggEnergyRegenAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    UE_LOG(LogTemp, Log, TEXT("EggEnergyRegen ability ended"));
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
