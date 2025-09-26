//EGTrickAbility.cpp

#include "AbilitySystem/Ability/EGTrickAbility.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/GameplayEffect/EGResetEggEnergyEffect.h"
#include "AbilitySystem/GameplayEffect/EGStunEffect.h"
#include "Character/EGChickenCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UEGTrickAbility::UEGTrickAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEGTrickAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("Trick Ability activate"));

	if (IsValid(TrickParticle))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TrickParticle,
			ActorInfo->AvatarActor->GetActorLocation(),
			ActorInfo->AvatarActor->GetActorRotation());
	}

	UAbilityTask_WaitGameplayEvent* WaitEvent =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			FGameplayTag::RequestGameplayTag("Event.EggBroken"));

	WaitEvent->EventReceived.AddDynamic(this, &UEGTrickAbility::OnEggBroken);
	WaitEvent->ReadyForActivation();
	
}

void UEGTrickAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGTrickAbility::OnEggBroken(FGameplayEventData EventData)
{
	if (const AActor* Instigator = EventData.Instigator.Get())
	{
		if (const AEGChickenCharacter* Character = Cast<AEGChickenCharacter>(Instigator))
		{
			if (UAbilitySystemComponent* TargetASC = Character->GetAbilitySystemComponent())
			{
				FGameplayEffectSpecHandle StunSpec = MakeOutgoingGameplayEffectSpec(UEGStunEffect::StaticClass(), 1.0f);
				FGameplayEffectSpecHandle ResetEnergySpec = MakeOutgoingGameplayEffectSpec(UEGResetEggEnergyEffect::StaticClass(), 1.0f);
				
				TargetASC->ApplyGameplayEffectSpecToSelf(*StunSpec.Data.Get());
				TargetASC->ApplyGameplayEffectSpecToSelf(*ResetEnergySpec.Data.Get());
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}