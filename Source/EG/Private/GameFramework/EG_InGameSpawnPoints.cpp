
#include "GameFramework/EG_InGameSpawnPoints.h"


// Sets default values
AEG_InGameSpawnPoints::AEG_InGameSpawnPoints()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnSortNumber = 0;
}
