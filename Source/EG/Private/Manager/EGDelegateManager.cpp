// EGDelegateManager.cpp

#include "Manager/EGDelegateManager.h"
#include "EGLog.h"

void UEGDelegateManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	EG_LOG(LogKH, Log, TEXT("Delegate Manager Initialized"));
}

void UEGDelegateManager::Deinitialize()
{
	EG_LOG(LogKH, Log, TEXT("Delegate Manager Deinitialized"));
	Super::Deinitialize();
}

void UEGDelegateManager::BroadcastItemPickUp(AEGItemBase* Item)
{
	OnItemPickUp.Broadcast(Item);
	EG_LOG(LogKH, Log, TEXT("OnItemPickUp Broadcasted"));
}
