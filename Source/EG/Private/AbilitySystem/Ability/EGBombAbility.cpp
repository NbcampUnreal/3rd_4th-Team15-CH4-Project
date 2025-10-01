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

	// TODO: JM / 현재 액터 파괴시 SFX 소리 재생이 안 됨(향후 오브젝트 풀링으로 바꿔야 할 필요 있음)
	if (AEggActor* OwnerEgg = Cast<AEggActor>(ActorInfo->AvatarActor.Get()))
	{
		OwnerEgg->Destroy(); // 폭탄 알은 여기서 파괴
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
