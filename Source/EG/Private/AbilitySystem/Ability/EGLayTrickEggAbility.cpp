//EGLayTrickEggAbility.cpp

#include "AbilitySystem/Ability/EGLayTrickEggAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/GameplayEffect/EGLayTrickEggCooldownEffect.h"
#include "Character/Egg/EggActor.h"

UEGLayTrickEggAbility::UEGLayTrickEggAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = nullptr;
	CooldownGameplayEffectClass = UEGLayTrickEggCooldownEffect::StaticClass();
}

void UEGLayTrickEggAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	if (IsValid(LayEggMontage))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, LayEggMontage, 1.0f,
				NAME_None, false, 1.0f);

		if (IsValid(PlayMontageTask))
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UEGLayTrickEggAbility::OnMontageFinished);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &UEGLayTrickEggAbility::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UEGLayTrickEggAbility::OnMontageCancelled);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UEGLayTrickEggAbility::OnMontageCancelled);

			PlayMontageTask->ReadyForActivation();
		}

		if (IsValid(EggActorClass))
		{
			if (GetOwningActorFromActorInfo()->HasAuthority())
			{
				FVector ActorLocation = ActorInfo->AvatarActor->GetActorLocation();
				FVector BackWardVector = -ActorInfo->AvatarActor->GetActorForwardVector();
				float SpawnDistance = 100.0f;
				FVector SpawnLocation = ActorLocation + BackWardVector * SpawnDistance;

				AEggActor* EggActor = GetWorld()->SpawnActor<AEggActor>(EggActorClass, ActorLocation,
				                                                        ActorInfo->AvatarActor->GetActorRotation());
				EggActor->SetOwner(ActorInfo->AvatarActor.Get());
			}
		}
	}
}

void UEGLayTrickEggAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       bool bReplicateEndAbility,
                                       bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGLayTrickEggAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGLayTrickEggAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
