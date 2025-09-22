// EGItem_ConfigData.h

#pragma once

#include "Engine/DataAsset.h"
#include "EGItem_ConfigData.generated.h"

class AEGItemBase;

USTRUCT(BlueprintType)
struct FItemSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEGItemBase> SpawnItem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Probability = 0.f;
};

/**
 * 
 */
UCLASS(BlueprintType)
class EG_API UEGItem_ConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemSpawnData> SpawnDataList;
};
