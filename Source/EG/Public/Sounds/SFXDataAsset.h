// SFXDataAsset.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "SFXDataType.h"
#include "Engine/DataAsset.h"
#include "SFXDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class EG_API USFXDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFX")
	TArray<FSFXInfo> SFXList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFX")
	float VolumeMultiplier = 1.0f;

	// 사운드마다 어테뉴에이션이 다를 필요는 없어서 이곳에 배치
	// 만약 달라야 하면, FSFXInfo에 넣으면 됨
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFX")
	TObjectPtr<USoundAttenuation> SFXAttenuation;	

	USoundBase* GetSFXSound(ESFXType InType) const
	{
		const FSFXInfo* Found = SFXList.FindByPredicate(
			[InType](const FSFXInfo& Info)
			{
				return Info.SFXType == InType;
			});
		return Found ? Found->SFXSound : nullptr;
	}
};
