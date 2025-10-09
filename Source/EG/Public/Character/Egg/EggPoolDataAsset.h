// EggPoolDataAsset.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "EEggType.h"
#include "EggActor.h"
#include "Engine/DataAsset.h"
#include "EggPoolDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FEggPoolInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEggType EggType = EEggType::Egg;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEggActor> EggBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PoolSize = 5;
};

UCLASS(BlueprintType)
class EG_API UEggPoolDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEggPoolInfo> EggPoolList;
};
