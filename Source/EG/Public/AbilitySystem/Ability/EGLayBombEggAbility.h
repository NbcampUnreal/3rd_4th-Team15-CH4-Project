//EGLayBombEggAbility.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGLayBombEggAbility.generated.h"

UCLASS()
class EG_API UEGLayBombEggAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGLayBombEggAbility();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LayEgg")
	TObjectPtr<UAnimMontage> LayEggMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LayEgg")
	TSubclassOf<AActor> EggActorClass;
	
};
