//EGSprintAbility.cpp

#include "AbilitySystem/Ability/EGSprintAbility.h"
#include "AbilitySystem/AttributeSet/EGCharacterAttributeSet.h"
#include "AbilitySystem/GameplayEffect/EGSprintCostEffect.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChangeThreshold.h"

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

	UE_LOG(LogTemp, Log, TEXT("Sprint Ability start"));

	AEGChickenCharacter* Character = CastChecked<AEGChickenCharacter>(ActorInfo->AvatarActor.Get());
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	FGameplayTag SprintTag = FGameplayTag::RequestGameplayTag(TEXT("Status.Sprint"));

	if (IsValid(Character) && IsValid(ASC))
	{
		ASC->AddLooseGameplayTag(SprintTag);
	}

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
			SprintCostEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SprintCostSpec);
		}
	}

	UAbilityTask_WaitAttributeChangeThreshold* WaitStamina =
		UAbilityTask_WaitAttributeChangeThreshold::WaitForAttributeChangeThreshold(
			this,
			UEGCharacterAttributeSet::GetStaminaAttribute(),
			EWaitAttributeChangeComparison::LessThan,
			20.0f,
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
	if (GetOwningActorFromActorInfo()->HasAuthority())
	{
		if (ActorInfo->AbilitySystemComponent.IsValid())
		{
			ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(SprintEffectHandle);
			ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(SprintCostEffectHandle);
		}
	}

	AEGChickenCharacter* Character = CastChecked<AEGChickenCharacter>(ActorInfo->AvatarActor.Get());
	if (IsValid(Character))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	}

	FGameplayTag SprintTag = FGameplayTag::RequestGameplayTag(TEXT("Status.Sprint"));
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->RemoveLooseGameplayTag(SprintTag);
	}

	UE_LOG(LogTemp, Log, TEXT("Sprint Ability end"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UEGSprintAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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
	if (CurrentStamina < 20.0f)
	{
		return false;
	}

	return true;
}

void UEGSprintAbility::OnStaminaTooLow(bool bMatchesComparison, float CurrentValue)
{
	UE_LOG(LogTemp, Log, TEXT("CurrentStamina : %.1f"), CurrentValue);
	if (bMatchesComparison)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
