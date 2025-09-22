// EGItem_OutlineAbility.h

#pragma once

#include "Abilities/GameplayAbility.h"
#include "EGItem_OutlineAbility.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGItem_OutlineAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEGItem_OutlineAbility();

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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Outline|Material")
	UMaterialInterface* OutlineMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Outline|Material")
	UMaterialInterface* OutlineMaterial_Clip;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial_Clip;

	UPROPERTY(EditDefaultsOnly, Category = "Outline|Property")
	float Opacity;

	UPROPERTY(EditDefaultsOnly, Category = "Outline|Property")
	FLinearColor Color;
	
	UPROPERTY(EditDefaultsOnly, Category = "Outline|Property")
	float OutlineOpacity;
	
	UPROPERTY(EditDefaultsOnly, Category = "Outline|Property")
	FLinearColor OutlineColor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Outline|GAS")
	float AbilityDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Outline|GAS")
	TSubclassOf<AActor> OutlineActorClass;

	UPROPERTY()
	TArray<AActor*> OutlinedActors;

	UFUNCTION()
	void OnDurationFinished();

	void SetMaterial(ACharacter* InCharacter, UMaterialInterface* InMaterial, UMaterialInstanceDynamic* InDynamicMaterial);
	void SetCustomDepth(AActor* Target, bool bIsCustomDepth);
};
