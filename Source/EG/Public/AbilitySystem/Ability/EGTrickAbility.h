//EGTrickAbility.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGTrickAbility.generated.h"

class UNiagaraSystem;

UCLASS()
class EG_API UEGTrickAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGTrickAbility();

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
	TObjectPtr<UNiagaraSystem> TrickParticle;
};