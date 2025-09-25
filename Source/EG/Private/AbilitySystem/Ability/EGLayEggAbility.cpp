//EGLayEggAbility.cpp

#include "AbilitySystem/Ability/EGLayEggAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "AbilitySystem/GameplayEffect/EGLayEggCooldownEffect.h"
#include "Character/Egg/EggActor.h"
#include "AbilitySystem/GameplayEffect/EGResetEggEnergyEffect.h"

UEGLayEggAbility::UEGLayEggAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = nullptr;
	CooldownGameplayEffectClass = UEGLayEggCooldownEffect::StaticClass();
}

void UEGLayEggAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	FGameplayEffectSpecHandle ResetEnergySpec = MakeOutgoingGameplayEffectSpec(UEGResetEggEnergyEffect::StaticClass(), 1.0f);
	ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*ResetEnergySpec.Data.Get());

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
				FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();

				AEggActor* EggActor = GetWorld()->SpawnActor<AEggActor>(EggActorClass, SpawnLocation,
				                                                        ActorInfo->AvatarActor->GetActorRotation());

				UE_LOG(LogTemp, Log, TEXT("Egg Spawned"));
			}
		}
	}
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

bool UEGLayEggAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayTagContainer* SourceTags,
                                          const FGameplayTagContainer* TargetTags,
                                          FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const UEGCharacterAttributeSet* AttributeSet = ActorInfo->AbilitySystemComponent->GetSet<UEGCharacterAttributeSet>();
	if (!AttributeSet)
	{
		return false;
	}

	if (AttributeSet->GetEggEnergy() < 70.0f)
	{
		return false;
	}

	return true;
}

void UEGLayEggAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UEGCharacterAttributeSet::GetEggEnergyAttribute())
			.AddUObject(this, &UEGLayEggAbility::OnEggEnergyChanged);
	}
}

void UEGLayEggAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UEGCharacterAttributeSet::GetEggEnergyAttribute())
			.RemoveAll(this);
	}

	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UEGLayEggAbility::OnEggEnergyChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue >= 100.0f)
	{
		GetAbilitySystemComponentFromActorInfo()->TryActivateAbility(GetCurrentAbilitySpecHandle());
	}
}

void UEGLayEggAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGLayEggAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
