
//#include "EG_Player"
#include "GameFramework/EGPlayerStart.h"



AEGPlayerStart::AEGPlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	PrimaryActorTick.bCanEverTick = false;
	PlayerIndex = 0;	
}

int32 AEGPlayerStart::GetPlayerIndex()
{
	return PlayerIndex;
}
