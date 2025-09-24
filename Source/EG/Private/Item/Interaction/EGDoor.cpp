// EGDoor.cpp

#include "Item/Interaction/EGDoor.h"

#include "Components/BoxComponent.h"

AEGDoor::AEGDoor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(Pivot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	StaticMesh->SetupAttachment(Pivot);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(Pivot);
	CollisionComponent->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	bIsOpen = false;
}

void AEGDoor::BeginPlay()
{
	Super::BeginPlay();

	ClosedRotation = Pivot->GetRelativeRotation();
	OpenRotation = ClosedRotation + FRotator(0, 90.f, 0);

	if (DoorCurve)
	{
		FOnTimelineFloat InterpFunction;
		InterpFunction.BindUFunction(this, FName("HandleDoor"));
		
		DoorTimeline->AddInterpFloat(DoorCurve, InterpFunction);
		DoorTimeline->SetLooping(false);
	}
}

void AEGDoor::Interact_Implementation()
{
	if (DoorTimeline->IsPlaying())
	{
		DoorTimeline->Reverse();
	}
	else
	{
		if (bIsOpen)
		{
			DoorTimeline->ReverseFromEnd();
		}
		else
		{
			DoorTimeline->PlayFromStart();
		}
	}
	
	bIsOpen = !bIsOpen;
}

void AEGDoor::HandleDoor(float Value)
{
	FRotator NewRotation = FMath::Lerp(ClosedRotation, OpenRotation, Value);
	Pivot->SetRelativeRotation(NewRotation);
}
