//EGSprintAbility.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGSprintAbility.generated.h"

UCLASS()
class EG_API UEGSprintAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGSprintAbility();

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

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                                const FGameplayAbilityActorInfo* ActorInfo,
	                                const FGameplayTagContainer* SourceTags = nullptr,
	                                const FGameplayTagContainer* TargetTags = nullptr,
	                                FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint")
	TSubclassOf<UGameplayEffect> SprintEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint")
	TSubclassOf<UGameplayEffect> SprintCostEffectClass;

private:
	UFUNCTION()
	void OnStaminaTooLow(bool bMatchesComparison, float CurrentValue);

	FGameplayEffectSpecHandle SprintSpec;
	FGameplayEffectSpecHandle SprintCostSpec;

	FActiveGameplayEffectHandle SprintEffectHandle;
	FActiveGameplayEffectHandle SprintCostEffectHandle;
};
