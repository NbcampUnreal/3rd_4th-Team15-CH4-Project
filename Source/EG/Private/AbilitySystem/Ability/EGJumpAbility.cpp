// EGJumpAbility.cpp (JJM)


#include "AbilitySystem/Ability/EGJumpAbility.h"

#include "AbilitySystemComponent.h"
#include "EGLog.h"
#include "Character/EGChickenCharacter.h"

UEGJumpAbility::UEGJumpAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEGJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EG_LOG(LogJM, Warning, TEXT("Can not Commit Ability"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AEGChickenCharacter* EGChicken = Cast<AEGChickenCharacter>(ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr);
	if (!EGChicken)
	{
		EG_LOG(LogJM, Warning, TEXT("Cast Failed <EGChickenCharacter>"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	EGChicken->Jump();		// StopJumping은 EGChickenCharacter::HandleStopJump()에서 실행함

	// GameplayCue Jump SFX
	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = ActorInfo->AvatarActor->GetActorLocation();
		ActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Status.Jump")), CueParams);
	}
	else
	{
		EG_LOG(LogJM, Warning, TEXT("Jump Ability AbilitySystemComponent is null"));
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
