
#include "GameFramework/EGInGameSpawnPoints.h"


// Sets default values
AEGInGameSpawnPoints::AEGInGameSpawnPoints()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnSortNum = 0;
	
}

int32 AEGInGameSpawnPoints::GetSpawnSortNum()
{
	return SpawnSortNum;
}
