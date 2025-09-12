// EGPeckAbility.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGPeckAbility.generated.h"

UCLASS()
class EG_API UEGPeckAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGPeckAbility();

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
	void OnMontageFinished();

	UFUNCTION()
	void OnMontageCancelled();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Peck")
	TSubclassOf<UGameplayEffect> PeckEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Peck")
	TObjectPtr<UAnimMontage> PeckMontage;
};
