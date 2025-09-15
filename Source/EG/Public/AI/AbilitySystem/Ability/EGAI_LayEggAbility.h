// EGAI_LayEggAbility.h

#pragma once

#include "Abilities/GameplayAbility.h"
#include "EGAI_LayEggAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class EG_API UEGAI_LayEggAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGAI_LayEggAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								const FGameplayAbilityActorInfo* ActorInfo,
								const FGameplayAbilityActivationInfo ActivationInfo,
								const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility,
							bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* LayEggMontage;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;

	UFUNCTION()
	void OnMontageFinished();
};
