// SFXManagerSubsystem.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "SFXDataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SFXManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class EG_API USFXManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USFXManagerSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void PlaySFXLocalClientOnly(ESFXType InType, UWorld* World);
	/*void PlaySFXAtLocation(ESFXType InType, UObject* WorldContext, const FVector& Location);*/
	void StopSFX(ESFXType InType);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<USFXDataAsset> SFXDataAsset;

	UPROPERTY()
	TMap<ESFXType, TObjectPtr<UAudioComponent>> ActiveSFXMap;

private:
	UAudioComponent* GetAudioComponent(UWorld* World, ESFXType InType);		// 재사용 or 새로만듦

};
