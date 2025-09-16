// EGPeckAbility.cpp

#include "AbilitySystem/Ability/EGPeckAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "AbilitySystem/GameplayEffect/EGPeckCooldownEffect.h"
#include "AbilitySystem/GameplayEffect/EGPeckEffect.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UEGPeckAbility::UEGPeckAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = nullptr;
	CooldownGameplayEffectClass = UEGPeckCooldownEffect::StaticClass();
	PeckEffectClass = UEGPeckEffect::StaticClass();
}

void UEGPeckAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Peck failed: Cooldown check failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
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

	if (IsValid(PeckEffectClass) && ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayEffectContextHandle ContextHandle = ActorInfo->AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
			PeckEffectClass, 1, ContextHandle);

		if (SpecHandle.IsValid())
		{
			ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			float CurrentEnergy = ActorInfo->AbilitySystemComponent->
			                                 GetNumericAttribute(UEGCharacterAttributeSet::GetEggEnergyAttribute());
			UE_LOG(LogTemp, Log, TEXT("CurrentEnergy: %0.1f"), CurrentEnergy);
		}
	}

}

void UEGPeckAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                bool bReplicateEndAbility,
                                bool bWasCancelled)
{
	UE_LOG(LogTemp, Log, TEXT("Peck Ability end"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGPeckAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGPeckAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
