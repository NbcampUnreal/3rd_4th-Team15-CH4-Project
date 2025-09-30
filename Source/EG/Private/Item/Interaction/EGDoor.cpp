// EGDoor.cpp

#include "Item/Interaction/EGDoor.h"

#include "EGLog.h"
#include "NavModifierComponent.h"
#include "Components/BoxComponent.h"
#include "NavAreas/NavArea_Default.h"
#include "NavAreas/NavArea_Null.h"
#include "Net/UnrealNetwork.h"

AEGDoor::AEGDoor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(Pivot);
	
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(Pivot);
	CollisionComponent->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	CollisionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	StaticMesh->SetupAttachment(CollisionComponent);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));
	NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
	NavModifier->SetAreaClass(UNavArea_Null::StaticClass());

	StaticMesh->SetCanEverAffectNavigation(false);
	
	bIsOpen = false;
	OpenRotation_Default = FRotator(0, 90.f, 0);
}

void AEGDoor::BeginPlay()
{
	Super::BeginPlay();

	ClosedRotation = Pivot->GetRelativeRotation();
	OpenRotation = ClosedRotation + OpenRotation_Default;

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
		if (!DoorTimeline->IsPlaying())
		{
			bIsOpen = !bIsOpen;
			OnRep_DoorState();

			// if (bIsOpen)
			// {
			// 	NavModifier->SetAreaClass(UNavArea_Default::StaticClass());
			// }
			// else
			// {
			// 	NavModifier->SetAreaClass(UNavArea_Null::StaticClass());
			// }
		}
	}
	else
	{
		// If needed, implement client-side logic
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
