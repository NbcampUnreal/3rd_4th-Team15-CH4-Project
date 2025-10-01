// EGItemBase.cpp

#include "Item/ItemBase/EGItemBase.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Character/EGChickenCharacter.h"
#include "Manager/EGDelegateManager.h"


AEGItemBase::AEGItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(CollisionComponent);
}

void AEGItemBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEGItemBase::OnOverlap);
}

void AEGItemBase::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AEGChickenCharacter::StaticClass()))
	{
		OnItemPickUp.Broadcast();
		
		Execute_OnPickUp(this, OtherActor);
	}
}

void AEGItemBase::OnPickUp_Implementation(AActor* PickUpActor)
{
	Destroy();
}
