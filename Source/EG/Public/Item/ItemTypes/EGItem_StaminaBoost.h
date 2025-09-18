// EGItem_StaminaBoost.h

#pragma once

#include "Item/ItemBase/EGItemBase.h"
#include "EGItem_StaminaBoost.generated.h"

class UGameplayEffect;

UCLASS()
class EG_API AEGItem_StaminaBoost : public AEGItemBase
{
	GENERATED_BODY()

public:
	AEGItem_StaminaBoost();

	// EGItemInterface
	virtual void OnPickUp_Implementation(AActor* PickUpActor) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item|GAS")
	TSubclassOf<UGameplayEffect> StaminaBoostEffectClass;
};
