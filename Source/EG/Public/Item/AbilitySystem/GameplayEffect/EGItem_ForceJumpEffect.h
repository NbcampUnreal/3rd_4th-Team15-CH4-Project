// EGItem_ForceJumpEffect.h

#pragma once

#include "GameplayEffect.h"
#include "EGItem_ForceJumpEffect.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGItem_ForceJumpEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UEGItem_ForceJumpEffect();

	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect")
	float EffectDuration;
};
