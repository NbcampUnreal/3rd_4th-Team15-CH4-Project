// EGItemInterface.h

#pragma once

#include "UObject/Interface.h"
#include "EGItemInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEGItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EG_API IEGItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void OnPickUp(AActor* PickUpActor);
};
