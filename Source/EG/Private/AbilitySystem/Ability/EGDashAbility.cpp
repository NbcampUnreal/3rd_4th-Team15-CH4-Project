//EGDashAbility.cpp

#include "AbilitySystem/Ability/EGDashAbility.h"

#include "AbilitySystem/GameplayEffect/EGDashCooldownEffect.h"
#include "AbilitySystem/GameplayEffect/EGDashCostEffect.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UEGDashAbility::UEGDashAbility()
{
	CostGameplayEffectClass = UEGDashCostEffect::StaticClass();
	CooldownGameplayEffectClass = UEGDashCooldownEffect::StaticClass();
}

void UEGDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Char = Cast<ACharacter>(ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr);
	if (!Char)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector Direction = Char->GetActorForwardVector().GetSafeNormal();
	const float Strength = (DashDuration > 0.f) ? (DashDistance / DashDuration) : DashDistance;

	UAbilityTask_ApplyRootMotionConstantForce* RMTask =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this, NAME_None, Direction, Strength, DashDuration, false, nullptr,
			ERootMotionFinishVelocityMode::MaintainLastRootMotionVelocity,
			FVector::ZeroVector, 0.0f, true);

	if (RMTask)
	{
		RMTask->ReadyForActivation();
	}

	if (UAbilityTask_WaitDelay* Delay = UAbilityTask_WaitDelay::WaitDelay(this, DashDuration))
	{
		Delay->OnFinish.AddDynamic(this, &UEGDashAbility::OnDashFinished);
		Delay->ReadyForActivation();
	}
}

void UEGDashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Log, TEXT("Dash Ability end"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGDashAbility::OnDashFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
