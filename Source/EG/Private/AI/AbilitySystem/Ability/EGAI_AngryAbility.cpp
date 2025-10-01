// EGAI_AngryAbility.cpp

#include "AI/AbilitySystem/Ability/EGAI_AngryAbility.h"

#include "AIController.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/EGAIState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UEGAI_AngryAbility::UEGAI_AngryAbility()
	: AngryMontage(nullptr)
	, MontageTask(nullptr)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEGAI_AngryAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Angry"), AngryMontage);
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageFinished_Completed);
		MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageFinished_Cancelled);
		MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageFinished_Cancelled);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UEGAI_AngryAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!bWasCancelled)
	{
		if (AAIController* AIController = Cast<AAIController>(ActorInfo->OwnerActor->GetInstigatorController()))
		{
			if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
			{
				BB->SetValueAsEnum("ActionState", static_cast<uint8>(EAIState::Idle));
			}
		}
	}
		
}

void UEGAI_AngryAbility::OnMontageFinished_Completed()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGAI_AngryAbility::OnMontageFinished_Cancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
