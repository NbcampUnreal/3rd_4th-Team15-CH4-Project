//EGLayBombEggAbility.cpp

#include "AbilitySystem/Ability/EGLayBombEggAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/GameplayEffect/EGLayBombEggCooldownEffect.h"
#include "Character/Egg/EggActor.h"

UEGLayBombEggAbility::UEGLayBombEggAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = nullptr;
	CooldownGameplayEffectClass = UEGLayBombEggCooldownEffect::StaticClass();
}

void UEGLayBombEggAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
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
			PlayMontageTask->OnCompleted.AddDynamic(this, &UEGLayBombEggAbility::OnMontageFinished);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &UEGLayBombEggAbility::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UEGLayBombEggAbility::OnMontageCancelled);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UEGLayBombEggAbility::OnMontageCancelled);

			PlayMontageTask->ReadyForActivation();
		}

		if (IsValid(EggActorClass))
		{
			if (GetOwningActorFromActorInfo()->HasAuthority())
			{
				FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();

				AEggActor* EggActor = GetWorld()->SpawnActor<AEggActor>(EggActorClass, SpawnLocation,
																		ActorInfo->AvatarActor->GetActorRotation());
				EggActor->SetOwner(ActorInfo->AvatarActor.Get());
			}
		}
	}
}

void UEGLayBombEggAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGLayBombEggAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGLayBombEggAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
