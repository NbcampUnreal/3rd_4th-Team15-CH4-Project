// EGItem_ForceJumpAbility.cpp

#include "Item/AbilitySystem/Ability/EGItem_ForceJumpAbility.h"

#include "AbilitySystemComponent.h"
#include "EGLog.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/Character.h"

UEGItem_ForceJumpAbility::UEGItem_ForceJumpAbility()
	: AbilityDuration(3.0f)
	, JumpInterval(0.2f)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Debuff.ForceJump")));
}

void UEGItem_ForceJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		GetWorld()->GetTimerManager().SetTimer(
			JumpTimerHandle,
			FTimerDelegate::CreateUObject(this, &ThisClass::ForceJump, Character),
			JumpInterval,
			true);

		GetWorld()->GetTimerManager().SetTimer(
			DurationTimerHandle,
			this,
			&ThisClass::OnDurationFinished,
			AbilityDuration,
			false);
	}
}

void UEGItem_ForceJumpAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (HasAuthority(&ActivationInfo))
	{
		GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);
		
		if (ActorInfo->AbilitySystemComponent.IsValid())
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGItem_ForceJumpAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->StopJumping();
	}
}

void UEGItem_ForceJumpAbility::OnDurationFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
	
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}

void UEGItem_ForceJumpAbility::ForceJump(ACharacter* Character)
{
	if (Character && Character->CanJump())
	{
		// Character->Jump();
		AEGChickenCharacter* EGChicken = Cast<AEGChickenCharacter>(Character);
		if (EGChicken)
		{
			// JM : 점프시 소리 나도록 Ability 실행하는 래핑 함수 실행
			EGChicken->ExecuteJump();
		}
		else
		{
			EG_LOG(LogJM, Warning, TEXT("Cast Failed <AEGChickenCharacter>"));
		}
	}
}
