//EGLayEggAbility.cpp

#include "AbilitySystem/Ability/EGLayEggAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "AbilitySystem/GameplayEffect/EGLayEggCooldownEffect.h"
#include "AbilitySystem/GameplayEffect/EGLayEggCostEffect.h"
#include "Character/Egg/EggActor.h"

UEGLayEggAbility::UEGLayEggAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = UEGLayEggCostEffect::StaticClass();
	CooldownGameplayEffectClass = UEGLayEggCooldownEffect::StaticClass();
}

void UEGLayEggAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("LayEgg Ability start"));
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("LayEgg failed: Cost check failed"));
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
			PlayMontageTask->OnCompleted.AddDynamic(this, &UEGLayEggAbility::OnMontageFinished);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &UEGLayEggAbility::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UEGLayEggAbility::OnMontageCancelled);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UEGLayEggAbility::OnMontageCancelled);

			PlayMontageTask->ReadyForActivation();
		}

		if (IsValid(EggActorClass))
		{
			if (GetOwningActorFromActorInfo()->HasAuthority())
			{
				// 서버에서만 알 스폰
				const float SpawnDistance = 100.0f;
				FVector ActorLocation = ActorInfo->AvatarActor->GetActorLocation();
				FVector BackwardVector = -ActorInfo->AvatarActor->GetActorForwardVector();
				FVector SpawnLocation = ActorLocation + BackwardVector * SpawnDistance;

				AEggActor* EggActor = GetWorld()->SpawnActor<AEggActor>(EggActorClass, SpawnLocation,
				                                                        ActorInfo->AvatarActor->GetActorRotation());

				UE_LOG(LogTemp, Log, TEXT("Egg Spawned"));
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("LayEgg end"));
}

void UEGLayEggAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  bool bReplicateEndAbility,
                                  bool bWasCancelled)
{
	UE_LOG(LogTemp, Log, TEXT("LayEgg Ability end"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGLayEggAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGLayEggAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
