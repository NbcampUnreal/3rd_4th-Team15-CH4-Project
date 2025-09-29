//EGBombAbility.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGBombAbility.generated.h"

class UNiagaraSystem;

UCLASS()
class EG_API UEGBombAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGBombAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility,
							bool bWasCancelled) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Particle)
	TObjectPtr<UNiagaraSystem> ExplosionParticle;
	
};
