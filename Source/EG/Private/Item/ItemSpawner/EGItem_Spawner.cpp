// EGItem_Spawner.cpp

#include "Item/ItemSpawner/EGItem_Spawner.h"

#include "EGLog.h"
#include "Item/ItemBase/EGItemBase.h"
#include "Item/ItemSpawner/EGItem_ConfigData.h"
#include "Manager/EGDelegateManager.h"

AEGItem_Spawner::AEGItem_Spawner()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
}

void AEGItem_Spawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnItem();
	}
}

void AEGItem_Spawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(EndPlayReason);
}

void AEGItem_Spawner::SpawnItem()
{
	if (HasAuthority())
	{
		if (SpawnedItem)
		{
			return;
		}
		
		TSubclassOf<AEGItemBase> ItemClass = GetRandomItemClass();
		if (!ItemClass)
		{
			return;
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SpawnedItem = GetWorld()->SpawnActor<AEGItemBase>(ItemClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (SpawnedItem)
		{
			SpawnedItem->OnItemPickUp.AddDynamic(this, &ThisClass::HandleItemPickUp);
		}
	}
}

void AEGItem_Spawner::HandleItemPickUp()
{
	UE_LOG(LogTemp, Log, TEXT("Item Picked Up"));
	if (HasAuthority())
	{
		SpawnedItem = nullptr;

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::SpawnItem, ItemConfigData->SpawnInterval, false);
	}
}

TSubclassOf<AEGItemBase> AEGItem_Spawner::GetRandomItemClass()
{
	float TotalWeight = 0.f;
	for (auto& Entry : ItemConfigData->SpawnDataList)
	{
		TotalWeight += Entry.Probability;
	}

	if (TotalWeight <= 0.f) return nullptr;

	float RandomValue = FMath::FRandRange(0.f, TotalWeight);
	float AccumulatedWeight = 0.f;
	for (auto& Entry : ItemConfigData->SpawnDataList)
	{
		AccumulatedWeight += Entry.Probability;
		if (RandomValue <= AccumulatedWeight)
		{
			return Entry.SpawnItem;
		}
	}

	return nullptr;
}