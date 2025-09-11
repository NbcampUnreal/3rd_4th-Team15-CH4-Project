
//#include "EG_Player"
#include "GameFramework/EG_PlayerStart.h"



AEG_PlayerStart::AEG_PlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	PrimaryActorTick.bCanEverTick = false;
	PlayerIndex = 0;	
}

int32 AEG_PlayerStart::GetPlayerIndex()
{
	return PlayerIndex;
}
