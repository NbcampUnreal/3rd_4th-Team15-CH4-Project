#include "AbilitySystem/Ability/EGStaminaRegenAbility.h"

#include "AbilitySystem/GameplayEffect/EGStaminaRegenEffect.h"
#include "AbilitySystemComponent.h"

UEGStaminaRegenAbility::UEGStaminaRegenAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEGStaminaRegenAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(
				UEGStaminaRegenEffect::StaticClass(), 1.0f);

		if (SpecHandle.IsValid())
		{
			ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void UEGStaminaRegenAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateEndAbility,
                                        bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}