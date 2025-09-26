// EGInteractInterface.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EGInteractInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UEGInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EG_API IEGInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void Interact();
};
