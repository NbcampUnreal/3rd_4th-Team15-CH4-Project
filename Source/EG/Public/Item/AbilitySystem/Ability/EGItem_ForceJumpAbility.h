// EGItem_ForceJumpAbility.h

#pragma once

#include "Abilities/GameplayAbility.h"
#include "EGItem_ForceJumpAbility.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGItem_ForceJumpAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGItem_ForceJumpAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								const FGameplayAbilityActorInfo* ActorInfo,
								const FGameplayAbilityActivationInfo ActivationInfo,
								const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility,
							bool bWasCancelled);

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateCancelAbility) override;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	float AbilityDuration;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility")
	float JumpInterval;
	
private:
	FTimerHandle JumpTimerHandle;
	FTimerHandle DurationTimerHandle;
	
	UFUNCTION()
	void OnDurationFinished();

	UFUNCTION()
	void ForceJump(ACharacter* Character);
};
