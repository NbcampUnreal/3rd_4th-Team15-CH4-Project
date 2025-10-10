// EGGameplayCue_Status_Peck.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "EGGameplayCue.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class EG_API UEGGameplayCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
 	virtual void HandleGameplayCue(AActor* Target, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> SFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundAttenuation> SFXAttenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<UNiagaraSystem> NiagaraSystem;
};
