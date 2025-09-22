// EGItem_StaminaBoostEffect.h

#pragma once

#include "GameplayEffect.h"
#include "EGItem_StaminaBoostEffect.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGItem_StaminaBoostEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UEGItem_StaminaBoostEffect();

	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect")
	float BoostDuration;
};
