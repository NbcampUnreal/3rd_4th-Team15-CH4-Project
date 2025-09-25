//EGAttackAbility.cpp

#include "AbilitySystem/Ability/EGAttackAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/GameplayEffect/EGAttackCooldownEffect.h"
#include "AbilitySystem/GameplayEffect/EGResetEggEnergyEffect.h"
#include "AbilitySystem/GameplayEffect/EGStunEffect.h"
#include "AI/EGAICharacter.h"
#include "Character/EGChickenCharacter.h"
#include "Character/Egg/EggActor.h"
#include "Engine/OverlapResult.h"

UEGAttackAbility::UEGAttackAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	CostGameplayEffectClass = nullptr;
	CooldownGameplayEffectClass = UEGAttackCooldownEffect::StaticClass();
}

void UEGAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	if (IsValid(AttackMontage))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, AttackMontage, 1.0f,
				NAME_None, false, 1.0f);

		if (IsValid(PlayMontageTask))
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UEGAttackAbility::OnMontageFinished);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &UEGAttackAbility::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UEGAttackAbility::OnMontageCancelled);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UEGAttackAbility::OnMontageCancelled);

			PlayMontageTask->ReadyForActivation();

			const float SpawnDistance = 100.0f;
			FVector ActorLocation = ActorInfo->AvatarActor->GetActorLocation();
			FVector ForwardVector = ActorInfo->AvatarActor->GetActorForwardVector();

			FVector SpawnLocation = ActorLocation + ForwardVector * SpawnDistance;
			float SphereRadius = 100.0f;

			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(ActorInfo->AvatarActor.Get());

			bool bHit = GetWorld()->OverlapMultiByChannel(
				OverlapResults,
				SpawnLocation,
				FQuat::Identity,
				ECC_WorldDynamic,
				FCollisionShape::MakeSphere(SphereRadius),
				QueryParams);

			DrawDebugSphere(
				GetWorld(), // 월드 포인터
				SpawnLocation, // 중심 위치
				SphereRadius, // 반지름
				32, // 세그먼트 수
				bHit ? FColor::Green : FColor::Red, // 충돌 시 초록색, 미충돌 시 빨간색
				false, // 지속적으로 그릴지 여부
				3.0f, // 표시 지속 시간 (초)
				0, // 우선순위
				1.0f);
			
			if (bHit)
			{
				TSet<AActor*> UniqueActors;
				
				for (const FOverlapResult& Result : OverlapResults)
				{
					if (AActor* HitActor = Result.GetActor())
					{
						UniqueActors.Add(HitActor);
					}
				}

				for (AActor* HitActor : UniqueActors)
				{
					if (AEggActor* Egg = Cast<AEggActor>(HitActor))
					{
						int32 Health = Egg->GetHealth() - 1;
						Egg->SetHealth(Health);
						Egg->CheckHealthAndDestroy();
					}
					else
					{
						FGameplayEffectSpecHandle StunSpec = MakeOutgoingGameplayEffectSpec(
							UEGStunEffect::StaticClass(), 1.0f);

						FGameplayEffectSpecHandle ResetEnergySpec = MakeOutgoingGameplayEffectSpec(
							UEGResetEggEnergyEffect::StaticClass(), 1.0f);

						if (AEGChickenCharacter* Character = Cast<AEGChickenCharacter>(HitActor))
						{
							UAbilitySystemComponent* TargetASC = Character->GetAbilitySystemComponent();
							TargetASC->ApplyGameplayEffectSpecToSelf(*StunSpec.Data.Get());
							TargetASC->ApplyGameplayEffectSpecToSelf(*ResetEnergySpec.Data.Get());
						}
						else if (AEGAICharacter* AICharacter = Cast<AEGAICharacter>(HitActor))
						{
							AICharacter->OnAngryMode();
							ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*StunSpec.Data.Get());
							ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
								*ResetEnergySpec.Data.Get());
						}
					}
				}
			}
		}
	}
}

void UEGAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  bool bReplicateEndAbility,
                                  bool bWasCancelled)
{
	UE_LOG(LogTemp, Log, TEXT("EndAbility : Attack Ability End"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEGAttackAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEGAttackAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
