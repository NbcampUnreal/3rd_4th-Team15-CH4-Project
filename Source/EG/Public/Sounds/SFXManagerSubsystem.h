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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void PlaySFXLocalClientOnly(ESFXType InType, UObject* WorldContext);
	void PlaySFXAtLocation(ESFXType InType, UObject* WorldContext, const FVector& Location);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<USFXDataAsset> SFXDataAsset;

	// BGM과 달리 즉발성이고 추가적인 관리가 필요 없어서 AudioComponent 안쓰고
	// UGameplayStatics로만 재생함
};
