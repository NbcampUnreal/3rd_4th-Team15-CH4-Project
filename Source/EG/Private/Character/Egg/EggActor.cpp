//EggActor.cpp

#include "Character/Egg/EggActor.h"

AEggActor::AEggActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootScene);
	
}

int32 AEggActor::GetHealth() const
{
	return Health;
}

void AEggActor::SetHealth(int32 NewHealth)
{
	Health = NewHealth;
}
