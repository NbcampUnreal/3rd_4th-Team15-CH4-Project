// EGItem_Spawner.cpp

#include "Item/ItemSpawner/EGItem_Spawner.h"

#include "Item/ItemBase/EGItemBase.h"
#include "Item/ItemSpawner/EGItem_ConfigData.h"

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

void AEGItem_Spawner::SpawnItem()
{
	if (HasAuthority())
	{
		TSubclassOf<AEGItemBase> ItemClass = GetRandomItemClass();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AEGItemBase>(ItemClass, GetActorLocation(), GetActorRotation(), SpawnParams);
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