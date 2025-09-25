// EGDoor.cpp

#include "Item/Interaction/EGDoor.h"

#include "EGLog.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

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
	CollisionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

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

void AEGDoor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEGDoor, bIsOpen);
}

void AEGDoor::Interact_Implementation()
{
	if (HasAuthority())
	{
		EG_LOG_NET(LogKH, Log, TEXT("Door Interact Start"));
		if (!DoorTimeline->IsPlaying())
		{
			bIsOpen = !bIsOpen;
			OnRep_DoorState();
		}
		EG_LOG_NET(LogKH, Log, TEXT("Door Interact End"));
	}
	else
	{
		EG_LOG_NET(LogKH, Log, TEXT("Door Interact Start"));
		//ServerRPC_SetDoorState(bNewState);
		EG_LOG_NET(LogKH, Log, TEXT("Door Interact End"));
	}
}

void AEGDoor::OnRep_DoorState()
{
	if (bIsOpen)
	{
		DoorTimeline->PlayFromStart();
	}
	else
	{
		DoorTimeline->ReverseFromEnd();
	}
}

void AEGDoor::HandleDoor(float Value)
{
	FRotator NewRotation = FMath::Lerp(ClosedRotation, OpenRotation, Value);
	Pivot->SetRelativeRotation(NewRotation);
}
