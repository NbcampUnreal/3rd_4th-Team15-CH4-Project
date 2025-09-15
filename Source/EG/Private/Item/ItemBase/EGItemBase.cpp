// EGItemBase.cpp

#include "Item/ItemBase/EGItemBase.h"

#include "AI/EGAICharacter.h"
#include "Components/SphereComponent.h"


AEGItemBase::AEGItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AEGItemBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEGItemBase::OnOverlap);
}

void AEGItemBase::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AEGAICharacter::StaticClass()))
	{
		OnPickUp(OtherActor);
	}
}

void AEGItemBase::OnPickUp_Implementation(AActor* PickUpActor)
{
	Destroy();
}
