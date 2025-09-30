// EGPeckAbility.cpp

#include "AbilitySystem/Ability/EGPeckAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GameplayEffect/EGPeckCooldownEffect.h"
#include "AbilitySystem/GameplayEffect/EGPeckEffect.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UEGPeckAbility::UEGPeckAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = nullptr;
	CooldownGameplayEffectClass = UEGPeckCooldownEffect::StaticClass();

	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Cooldown.Peck"));
}

void UEGPeckAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (IsValid(PeckMontage))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, PeckMontage, 1.0f,
				NAME_None, false, 1.0f);

		if (IsValid(PlayMontageTask))
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UEGPeckAbility::OnMontageFinished);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &UEGPeckAbility::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UEGPeckAbility::OnMontageCancelled);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UEGPeckAbility::OnMontageCancelled);

			PlayMontageTask->ReadyForActivation();
		}
	}

	AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
}

void UEGPeckAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                bool bReplicateEndAbility,
                                bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGPeckAbility::OnMontageFinished()
{
	if (IsValid(AbilitySystemComponent))
	{
		FGameplayEffectSpecHandle PeckSpec = MakeOutgoingGameplayEffectSpec(
			UEGPeckEffect::StaticClass(), 1.0f);

		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PeckSpec.Data.Get());
		UE_LOG(LogTemp, Warning, TEXT("Peck Ability GO"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Montage Finished"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGPeckAbility::OnMontageCancelled()
{
	UE_LOG(LogTemp, Warning, TEXT("Montage Cancelled"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
