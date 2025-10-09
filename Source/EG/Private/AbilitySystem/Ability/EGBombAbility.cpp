//EGBombAbility.cpp

#include "AbilitySystem/Ability/EGBombAbility.h"

#include "AbilitySystemComponent.h"
#include "EGLog.h"
#include "AbilitySystem/GameplayEffect/EGStunEffect.h"
#include "Character/EGChickenCharacter.h"
#include "Character/Egg/EggActor.h"
#include "Engine/OverlapResult.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/GameplayEffect/EGResetEggEnergyEffect.h"
#include "Character/Egg/EggPoolManagerSubsystem.h"

UEGBombAbility::UEGBombAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEGBombAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsValid(ExplosionParticle))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionParticle,
			ActorInfo->AvatarActor->GetActorLocation(),
			ActorInfo->AvatarActor->GetActorRotation());
	}
	
	FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();
	float SphereRadius = 200.0f;

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
				Egg->ApplyDamageAndCheckDestroy(1, ActorInfo->AvatarActor.Get());
			}
			else if (AEGChickenCharacter* Character = Cast<AEGChickenCharacter>(HitActor))
			{
				if (UAbilitySystemComponent* TargetASC = Character->GetAbilitySystemComponent())
				{
					FGameplayEffectSpecHandle StunSpec = MakeOutgoingGameplayEffectSpec(
						UEGStunEffect::StaticClass(), 1.0f);
					FGameplayEffectSpecHandle ResetEnergySpec = MakeOutgoingGameplayEffectSpec(
						UEGResetEggEnergyEffect::StaticClass(), 1.0f);

					TargetASC->ApplyGameplayEffectSpecToSelf(*StunSpec.Data.Get());
					TargetASC->ApplyGameplayEffectSpecToSelf(*ResetEnergySpec.Data.Get());
				}
			}
		}
	}
	// JM : GameplayCue Bomb SFX
	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = ActorInfo->AvatarActor->GetActorLocation();
		ActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(FName("GameplayCue.Status.Bomb")), CueParams);
	}
	else
	{
		EG_LOG(LogJM, Warning, TEXT("ASC Is Not Valid"));
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

	if (AEggActor* OwnerEgg = Cast<AEggActor>(ActorInfo->AvatarActor.Get()))
	{
		// OwnerEgg->Destroy(); // 폭탄 알은 여기서 파괴
		// JM : 오브젝트 풀에 반납
		if (UEggPoolManagerSubsystem* PoolManager = GetWorld()->GetSubsystem<UEggPoolManagerSubsystem>())
		{
			PoolManager->ReturnEggToPool(OwnerEgg);
		}
		else
		{
			EG_LOG(LogJM, Warning, TEXT("No Pool Manager"));
		}
	}
}

void UEGBombAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                bool bReplicateEndAbility,
                                bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
