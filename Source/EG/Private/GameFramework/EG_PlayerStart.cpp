
//#include "EG_Player"
#include "GameFramework/EG_PlayerStart.h"



AEG_PlayerStart::AEG_PlayerStart()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void AEG_PlayerStart::BeginPlay()
{
	Super::BeginPlay();

	PlayerIndex = 0;
}

int AEG_PlayerStart::GetPlayerIndex()
{
	return PlayerIndex;
}
