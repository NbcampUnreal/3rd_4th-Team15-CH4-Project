// EGItem_OutlineActor.h

#pragma once

#include "Item/ItemBase/EGItemBase.h"
#include "EGItem_OutlineActor.generated.h"

class UGameplayAbility;

UCLASS()
class EG_API AEGItem_OutlineActor : public AEGItemBase
{
	GENERATED_BODY()

public:
	AEGItem_OutlineActor();

	// EGItemInterface
	virtual void OnPickUp_Implementation(AActor* PickUpActor) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item|GAS")
	TSubclassOf<UGameplayAbility> OutlineAbilityClass;
};
