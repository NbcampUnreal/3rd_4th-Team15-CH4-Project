// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGStaminaRegenAbility.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGStaminaRegenAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
    UEGStaminaRegenAbility();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaminaRegen")
    TSubclassOf<class UGameplayEffect> StaminaRegenEffectClass;
	
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled
    ) override;
};
