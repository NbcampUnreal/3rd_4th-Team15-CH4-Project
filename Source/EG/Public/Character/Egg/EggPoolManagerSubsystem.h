// EggPoolManagerSubsystem.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "EggActor.h"
#include "EggPoolDataAsset.h"
#include "Subsystems/WorldSubsystem.h"
#include "EggPoolManagerSubsystem.generated.h"

/**
 * Actor가 아닌 UWorldSubsystem으로 한 이유
 * 1. 알은 특정 월드에 존재하고, 서버에서만 Spawn/Pool 관리를 함.
 *   - UWorldSubsystem은 World마다 인스턴스가 생기므로 멀티레벨/멀티 월드 상황에서도 자연스럽게 월드에 종속됨
 *   - 서버에서 생성하고, 각 클라가 리플리케이트 하는 방식
 * 2. 서버/클라 접근 용이
 *   - GetWorld()->GetSubsystem<UEggPoolManagerSubsystem>()으로 어느 클래스에서도 접근 가능
 * 3. 액터 대비 비용 절감
 *   - Manager를 별도 Spawn 없이 바로 사용 가능
 *   - Manager를 액터로 만들면 Replication 설정 관리 필요
 *   - Tick이나 물리 연산이 필요하지 않음(불필요한 Overhead)
 * 4. 템플릿 기반
 *   - 템플릿 기반으로 EggActor, BombEggActor, TrickEggActor 코드 통합
 */

USTRUCT()
struct FEggPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AEggActor*> PooledEggs;

	TArray<int32> FreeIndices;
};

UCLASS()
class EG_API UEggPoolManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void InitPools(UEggPoolDataAsset* InDataAsset);

	AEggActor* GetEggFromPool(EEggType InType, const FVector& InSpawnLocation, const FRotator& InSpawnRotation);
	void ReturnEggToPool(AEggActor* InEgg);

private:
	void CreatePool(FEggPoolInfo InPoolInfo);
	void DisableActor(AEggActor* InActor);
	void EnableActor(AEggActor* InActor, const FVector& InSpawnLocation, const FRotator& InSpawnRotation);
	
private:
	UPROPERTY()
	TObjectPtr<UEggPoolDataAsset> PoolDataAsset;

	UPROPERTY()
	TMap<EEggType, FEggPool> EggPoolMap;

	UPROPERTY()
	TMap<EEggType, TSubclassOf<AEggActor>> EggBPClasseMap;

	
};
