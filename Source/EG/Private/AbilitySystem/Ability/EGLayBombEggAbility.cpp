//EGLayBombEggAbility.cpp

#include "AbilitySystem/Ability/EGLayBombEggAbility.h"

#include "AbilitySystemComponent.h"
#include "EGLog.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/GameplayEffect/EGLayBombEggCooldownEffect.h"
#include "Character/Egg/BombEggActor.h"
#include "Character/Egg/EggActor.h"
#include "Character/Egg/EggPoolManagerSubsystem.h"

UEGLayBombEggAbility::UEGLayBombEggAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = nullptr;
	CooldownGameplayEffectClass = UEGLayBombEggCooldownEffect::StaticClass();
}

void UEGLayBombEggAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	if (IsValid(LayEggMontage))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, LayEggMontage, 1.0f,
				NAME_None, false, 1.0f);

		if (IsValid(PlayMontageTask))
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UEGLayBombEggAbility::OnMontageFinished);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &UEGLayBombEggAbility::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UEGLayBombEggAbility::OnMontageCancelled);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UEGLayBombEggAbility::OnMontageCancelled);

			PlayMontageTask->ReadyForActivation();
		}
		
		// JM : GameplayCue Lay Egg SFX
		if (ActorInfo->AbilitySystemComponent.IsValid())
		{
			FGameplayCueParameters CueParams;
			CueParams.Location = ActorInfo->AvatarActor->GetActorLocation();
			ActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Status.LayEgg")), CueParams);
		}
		else
		{
			EG_LOG(LogJM, Warning, TEXT("ASC Is Not Valid"));
		}

		if (IsValid(EggActorClass))
		{
			if (GetOwningActorFromActorInfo()->HasAuthority())
			{
				/*FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();

				AEggActor* EggActor = GetWorld()->SpawnActor<AEggActor>(EggActorClass, SpawnLocation,
				                                                        ActorInfo->AvatarActor->GetActorRotation());
				EggActor->SetOwner(ActorInfo->AvatarActor.Get());*/

				// JM : 오브젝트 풀에서 가져오기
				if (UEggPoolManagerSubsystem* PoolManager = GetWorld()->GetSubsystem<UEggPoolManagerSubsystem>())
				{
					FRotator SpawnRotation = ActorInfo->AvatarActor->GetActorRotation();
					FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();
					AEggActor* EggActor = PoolManager->GetEggFromPool(EEggType::BombEgg, SpawnLocation, SpawnRotation);
					EggActor->SetOwner(ActorInfo->AvatarActor.Get());
					ABombEggActor* BombEggActor = Cast<ABombEggActor>(EggActor);
					if (BombEggActor)
					{
						BombEggActor->StartExplosionTimer();		// JM : 폭발 타이머 시작
					}
					else
					{
						EG_LOG(LogJM, Warning, TEXT("No BombEggActor"));
					}
				}
				else
				{
					EG_LOG(LogJM, Warning, TEXT("No Pool Manager"));
				}
			}
		}
	}
}

void UEGLayBombEggAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility,
                                      bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGLayBombEggAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGLayBombEggAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
