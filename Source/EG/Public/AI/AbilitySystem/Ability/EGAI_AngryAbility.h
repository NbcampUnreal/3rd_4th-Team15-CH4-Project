// EGAI_AngryAbility.h

#pragma once

#include "Abilities/GameplayAbility.h"
#include "EGAI_AngryAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class EG_API UEGAI_AngryAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGAI_AngryAbility();

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
	UAnimMontage* AngryMontage;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* MontageTask;

	UFUNCTION()
	void OnMontageFinished();
};
