// EGGameplayCue_Actor.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "EGGameplayCue_Actor.generated.h"

/**
 * 
 */
UCLASS()
class EG_API AEGGameplayCue_Actor : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()
public:
	virtual void HandleGameplayCue(AActor* Target, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	/*virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual void GameplayCueFinishedCallback() override;*/

protected:
	void PlaySFXLoop(AActor* MyTarget);
	void StopSFX();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<USoundBase> SFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundAttenuation> SFXAttenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	float VolumeMultiplier = 1.0f;
};
