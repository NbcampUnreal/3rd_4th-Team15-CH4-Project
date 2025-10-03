// EGJumpAbility.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGJumpAbility.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGJumpAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGJumpAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								const FGameplayAbilityActorInfo* ActorInfo,
								const FGameplayAbilityActivationInfo ActivationInfo,
								const FGameplayEventData* TriggerEventData) override;
};
