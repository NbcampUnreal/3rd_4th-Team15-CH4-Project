// EggPoolManagerSubsystem.cpp (JJM)


#include "Character/Egg/EggPoolManagerSubsystem.h"

#include "EGLog.h"

void UEggPoolManagerSubsystem::InitPools(UEggPoolDataAsset* InDataAsset)
{
	if (!InDataAsset)
	{
		EG_LOG(LogJM, Warning, TEXT("No In Data Asset"));
		return;
	}
	PoolDataAsset = InDataAsset;
	
	for (const FEggPoolInfo& PoolInfo : PoolDataAsset->EggPoolList)
	{
		CreatePool(PoolInfo);
	}
}

AEggActor* UEggPoolManagerSubsystem::GetEggFromPool(EEggType InEggType, const FVector& InSpawnLocation, const FRotator& InSpawnRotation)
{
	if (!EggPoolMap.Contains(InEggType))
	{
		EG_LOG(LogJM, Warning, TEXT("No Pool for %s"), *StaticEnum<EEggType>()->GetValueAsString(InEggType));
		return nullptr;
	}

	FEggPool& Pool = EggPoolMap[InEggType];
	
	if (Pool.FreeIndices.Num() == 0)	// Pool 에 남은 Egg가 없는 경우
	{
		// 풀 확장: EggBPClasseMap에서 클래스 가져와 새로 생성
		EG_LOG(LogJM, Warning, TEXT("Pool for %d is empty! now Expanding... (now size : %d)"), InEggType, Pool.PooledEggs.Num());
		if (EggBPClasseMap.Contains(InEggType))
		{
			TSubclassOf<AEggActor> EggBPClass = EggBPClasseMap[InEggType];
			UWorld* World = GetWorld();
			if (World)
			{
				AEggActor* NewEgg = World->SpawnActor<AEggActor>(EggBPClass);
				if (NewEgg)
				{
					EnableActor(NewEgg, InSpawnLocation, InSpawnRotation);
					int32 NewIndex = Pool.PooledEggs.Add(NewEgg);
					return NewEgg;
				}
				EG_LOG(LogJM, Warning, TEXT("Cant create new egg actor"));
			}
			else
			{
				EG_LOG(LogJM, Warning, TEXT("No World"));
			}
		}
		else
		{
			EG_LOG(LogJM, Warning, TEXT("No Egg BP Class for %s"), *StaticEnum<EEggType>()->GetValueAsString(InEggType));
		}
	}

	// Pool에 남은 Egg가 있는 경우
	int32 Index = Pool.FreeIndices.Pop();
	AEggActor* Egg = Pool.PooledEggs[Index];
	if (Egg)
	{
		EnableActor(Egg, InSpawnLocation, InSpawnRotation);
		return Egg;
	}
	
	EG_LOG(LogJM, Warning, TEXT("Cant get Egg From Pool"));
	return nullptr;
}

void UEggPoolManagerSubsystem::ReturnEggToPool(AEggActor* InEggActor)
{
	if (!InEggActor)
	{
		EG_LOG(LogJM, Warning, TEXT("No In Egg Actor"));
		return;
	}

	EEggType Type = InEggActor->EggType;
	if (!EggPoolMap.Contains(Type))
	{
		EG_LOG(LogJM, Warning, TEXT("No Pool for %s"), *StaticEnum<EEggType>()->GetValueAsString(Type));
		return;
	}

	FEggPool& Pool = EggPoolMap[Type];
	int32 Index = Pool.PooledEggs.IndexOfByKey(InEggActor);
	if (Index != INDEX_NONE && !Pool.FreeIndices.Contains(Index))	// 중복 방지
	{
		Pool.FreeIndices.Add(Index);	// 해당 Egg를 사용 가능함으로 전환
		DisableActor(InEggActor);
	}
}

void UEggPoolManagerSubsystem::CreatePool(FEggPoolInfo InPoolInfo)
{
	if (!InPoolInfo.EggBPClass)
	{
		EG_LOG(LogJM, Warning, TEXT("No Egg BP Class"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		EG_LOG(LogJM, Warning, TEXT("No World"));
		return;
	}

	FEggPool NewPool;
	EggBPClasseMap.Add(InPoolInfo.EggType, InPoolInfo.EggBPClass);

	for (int32 i=0; i<InPoolInfo.PoolSize; i++)
	{
		AEggActor* NewEggActor = World->SpawnActor<AEggActor>(InPoolInfo.EggBPClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (NewEggActor)
		{
			DisableActor(NewEggActor);
			NewPool.PooledEggs.Add(NewEggActor);
			NewPool.FreeIndices.Add(i);
		}
		else
		{
			EG_LOG(LogJM, Warning, TEXT("Cant create new egg actor"));
		}
	}

	EggPoolMap.Add(InPoolInfo.EggType, NewPool);
}

void UEggPoolManagerSubsystem::DisableActor(AEggActor* InActor)
{
	InActor->SetActorHiddenInGame(true);
	InActor->SetActorEnableCollision(false);
	InActor->StaticMesh->SetEnableGravity(false);
}

void UEggPoolManagerSubsystem::EnableActor(AEggActor* InActor, const FVector& InSpawnLocation, const FRotator& InSpawnRotation)
{
	InActor->SetActorHiddenInGame(false);
	InActor->SetActorEnableCollision(true);
	InActor->SetActorLocation(InSpawnLocation);
	InActor->SetActorRotation(InSpawnRotation);
	InActor->StaticMesh->SetEnableGravity(true);
}
