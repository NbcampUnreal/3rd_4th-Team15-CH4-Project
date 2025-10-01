// EGPeckAbility.cpp

#include "AbilitySystem/Ability/EGPeckAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GameplayEffect/EGPeckCooldownEffect.h"
#include "AbilitySystem/GameplayEffect/EGPeckEffect.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UEGPeckAbility::UEGPeckAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = nullptr;
	CooldownGameplayEffectClass = UEGPeckCooldownEffect::StaticClass();

	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Ability.Cooldown.Peck"));
}

bool UEGPeckAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
										const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayTagContainer* SourceTags,
										const FGameplayTagContainer* TargetTags,
										FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// Character Jump Check
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character && Character->GetCharacterMovement()->IsFalling())
	{
		return false;
	}

	// Force Jump Item Check
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Debuff.ForceJump"))))
		{
			return false;
		}
	}

	return true;
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

	if (IsValid(AbilitySystemComponent))
	{
		// JM : GameplayCue_Peck SFX
		FGameplayCueParameters CueParams;
		CueParams.Location = ActorInfo->AvatarActor->GetActorLocation();
		ActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Status.Peck")), CueParams);
	}	
}

void UEGPeckAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                bool bReplicateEndAbility,
                                bool bWasCancelled)
{
	if (!bWasCancelled)
	{
		if (IsValid(AbilitySystemComponent))
		{
			FGameplayEffectSpecHandle PeckSpec = MakeOutgoingGameplayEffectSpec(
				UEGPeckEffect::StaticClass(), 1.0f);

			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PeckSpec.Data.Get());
		}
	}
	
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
