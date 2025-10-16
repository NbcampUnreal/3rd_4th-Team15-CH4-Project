//EGDashAbility.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGDashAbility.generated.h"

UCLASS()
class EG_API UEGDashAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGDashAbility();

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

	UFUNCTION()
	void OnDashFinished();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashDuration = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashDistance = 500.0f;
	
};
