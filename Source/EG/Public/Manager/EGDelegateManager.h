// EGDelegateManager.h

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "EGDelegateManager.generated.h"

class AEGItemBase;

#pragma region DECLARE_DELEGATE

// Item
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickUp, AEGItemBase*, Item);

#pragma endregion

/**
 * 
 */
UCLASS()
class EG_API UEGDelegateManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

#pragma region DELEGATE
	
	UPROPERTY(BlueprintAssignable)
	FOnItemPickUp OnItemPickUp;

#pragma endregion 

#pragma region BROADCAST
	
	void BroadcastItemPickUp(AEGItemBase* Item);

#pragma endregion 
};
