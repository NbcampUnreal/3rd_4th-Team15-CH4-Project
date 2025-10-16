// EGItem_ForceJump.h

#pragma once

#include "Item/ItemBase/EGItemBase.h"
#include "EGItem_ForceJump.generated.h"

class UGameplayAbility;

UCLASS()
class EG_API AEGItem_ForceJump : public AEGItemBase
{
	GENERATED_BODY()

public:
	AEGItem_ForceJump();

	// EGItemInterface
	virtual void OnPickUp_Implementation(AActor* PickUpActor) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item|GAS")
	TSubclassOf<UGameplayAbility> ForceJumpAbilityClass;
};
