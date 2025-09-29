//EGTrickAbility.cpp

#include "AbilitySystem/Ability/EGTrickAbility.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/GameplayEffect/EGResetEggEnergyEffect.h"
#include "AbilitySystem/GameplayEffect/EGStunEffect.h"
#include "Character/EGChickenCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/Egg/EggActor.h"

UEGTrickAbility::UEGTrickAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag("Event.TrickEggBroken");
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UEGTrickAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (IsValid(TrickParticle))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TrickParticle,
			ActorInfo->AvatarActor->GetActorLocation(),
			ActorInfo->AvatarActor->GetActorRotation());
	}

	if (TriggerEventData && IsValid(TriggerEventData->Instigator))
	{
		if (const AEGChickenCharacter* Breaker = Cast<AEGChickenCharacter>(TriggerEventData->Instigator))
		{
			if (UAbilitySystemComponent* TargetASC = Breaker->GetAbilitySystemComponent())
			{
				FGameplayEffectSpecHandle StunSpec = MakeOutgoingGameplayEffectSpec(
					UEGStunEffect::StaticClass(), 1.0f);

				FGameplayEffectSpecHandle ResetEnergySpec = MakeOutgoingGameplayEffectSpec(
					UEGResetEggEnergyEffect::StaticClass(), 1.0f);

				if (StunSpec.IsValid() && ResetEnergySpec.IsValid())
				{
					TargetASC->ApplyGameplayEffectSpecToSelf(*StunSpec.Data.Get());
					TargetASC->ApplyGameplayEffectSpecToSelf(*ResetEnergySpec.Data.Get());
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

	if (AEggActor* OwnerEgg = Cast<AEggActor>(ActorInfo->AvatarActor.Get()))
	{
		OwnerEgg->Destroy(); // 함정 알은 여기서 파괴
	}
}

void UEGTrickAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}