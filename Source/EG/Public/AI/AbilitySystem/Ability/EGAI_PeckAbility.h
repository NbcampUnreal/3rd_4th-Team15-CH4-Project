// EGAI_PeckAbility.h

#pragma once

#include "Abilities/GameplayAbility.h"
#include "EGAI_PeckAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS()
class EG_API UEGAI_PeckAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGAI_PeckAbility();

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
	UAnimMontage* PeckMontage;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;

	UFUNCTION()
	void OnMontageFinished();
};
