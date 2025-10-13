// EGGameplayCue_LocalOnly.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "EGGameplayCue_LocalOnly.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UEGGameplayCue_LocalOnly : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual void HandleGameplayCue(AActor* Target, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

protected:
	void PlaySFXLoopLocalOnly(AActor* MyTarget);
	void StopSFXLocalOnly();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<USoundBase> SFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	float VolumeMultiplier = 1.0f;
};
