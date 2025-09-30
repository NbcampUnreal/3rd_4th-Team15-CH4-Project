//EGSprintAbility.cpp

#include "AbilitySystem/Ability/EGSprintAbility.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "AbilitySystem/GameplayEffect/EGSprintCostEffect.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChangeThreshold.h"
#include "AbilitySystem/GameplayEffect/EGStaminaRegenEffect.h"

UEGSprintAbility::UEGSprintAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEGSprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CanActivateAbility(Handle, ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (AEGChickenCharacter* Character = CastChecked<AEGChickenCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
		{
			if (IsValid(SprintEffectClass))
			{
				SprintSpec = MakeOutgoingGameplayEffectSpec(SprintEffectClass, 1.0f);

				if (SprintSpec.IsValid())
				{
					SprintEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SprintSpec);

					const UEGCharacterAttributeSet* AttributeSet = Cast<UEGCharacterAttributeSet>(
						ASC->GetAttributeSet(UEGCharacterAttributeSet::StaticClass()));
					if (IsValid(AttributeSet))
					{
						float MoveSpeed = AttributeSet->GetMoveSpeed();
						Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
					}
				}
			}

			if (IsValid(SprintCostEffectClass))
			{
				SprintCostSpec = MakeOutgoingGameplayEffectSpec(SprintCostEffectClass, 1.0f);

				if (SprintCostSpec.IsValid())
				{
					SprintCostEffectHandle = ApplyGameplayEffectSpecToOwner(
						Handle, ActorInfo, ActivationInfo, SprintCostSpec);
				}
			}

			FGameplayTag RegenTag = FGameplayTag::RequestGameplayTag(FName("Ability.Passive.StaminaRegen"));
			if (ASC->HasMatchingGameplayTag(RegenTag))
			{
				ASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(RegenTag));
			}
		}
	}

	UAbilityTask_WaitAttributeChangeThreshold* WaitStamina =
		UAbilityTask_WaitAttributeChangeThreshold::WaitForAttributeChangeThreshold(
			this,
			UEGCharacterAttributeSet::GetStaminaAttribute(),
			EWaitAttributeChangeComparison::LessThanOrEqualTo,
			0.0f,
			false
		);

	if (WaitStamina)
	{
		WaitStamina->OnChange.AddDynamic(this, &UEGSprintAbility::OnStaminaTooLow);
		WaitStamina->ReadyForActivation();
	}
}

void UEGSprintAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  bool bReplicateEndAbility,
                                  bool bWasCancelled)
{
	if (AEGChickenCharacter* Character = CastChecked<AEGChickenCharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
		{
			ASC->RemoveActiveGameplayEffect(SprintEffectHandle);
			ASC->RemoveActiveGameplayEffect(SprintCostEffectHandle);
			
			FGameplayEffectSpecHandle RegenSpec = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
				UEGStaminaRegenEffect::StaticClass(), 1.0f, ActorInfo->AbilitySystemComponent->MakeEffectContext());
			if (RegenSpec.IsValid())
			{
				ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*RegenSpec.Data.Get());
			}
		}
		
		Character->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UEGSprintAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayTagContainer* SourceTags,
                                          const FGameplayTagContainer* TargetTags,
                                          FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!IsValid(ASC))
	{
		return false;
	}

	const UEGCharacterAttributeSet* AttributeSet = Cast<UEGCharacterAttributeSet>(
		ASC->GetAttributeSet(UEGCharacterAttributeSet::StaticClass()));
	if (!IsValid(AttributeSet))
	{
		return false;
	}

	const float CurrentStamina = AttributeSet->GetStamina();
	if (CurrentStamina <= 0.0f)
	{
		return false;
	}

	return true;
}

void UEGSprintAbility::OnStaminaTooLow(bool bMatchesComparison, float CurrentValue)
{
	if (bMatchesComparison)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
