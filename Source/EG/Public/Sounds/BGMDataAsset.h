// BGMDataAsset.h (작성자 : JJM)

#pragma once

#include "CoreMinimal.h"
#include "BGMDataTypes.h"
#include "Engine/DataAsset.h"
#include "BGMDataAsset.generated.h"

UCLASS()
class EG_API UBGMDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="BGM")		
	TArray<FBGMInfo> BGMList;									// 레벨 별 BGM 정보 목록을 담은 배열

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
	float VolumeMultiplier = 1.0f;								// BGM 볼륨조절용

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
	float FadeTime = 1.0f;										// BGM Fade In/Out 시간
};
