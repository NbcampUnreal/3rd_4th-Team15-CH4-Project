// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EGEggEnergyRegenAbility.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGEggEnergyRegenAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
    UEGEggEnergyRegenAbility();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EggEnergyRegen")
    TSubclassOf<class UGameplayEffect> EggEnergyRegenEffectClass;

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
