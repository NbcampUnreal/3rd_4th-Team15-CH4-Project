// EGAICharacter.cpp

#include "AI/EGAICharacter.h"

AEGAICharacter::AEGAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AEGAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// Initialize random stream
		int32 Seed = GetUniqueID() ^ FDateTime::Now().GetMillisecond();
		RandomStream.Initialize(Seed);
	}
}
