#include "GameFrameWork/EGStartingButton.h"
#include "EGLog.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/EGGameStateBase.h"
#include "GameFramework/PlayerState.h"


AEGStartingButton::AEGStartingButton()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	CapsuleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleComponent->SetNotifyRigidBodyCollision(true);
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AEGStartingButton::OnOverlapBegin);

}

void AEGStartingButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap Begin"));
	if (OtherActor)
	{
		AEGChickenCharacter* Character = Cast<AEGChickenCharacter>(OtherActor);
		if (Character)
		{
			UE_LOG(LogTemp, Log, TEXT("Cast Player"));
			if (ACharacter* CharacterPtr = Cast<ACharacter>(Character))
			{
				if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(CharacterPtr->GetController()))
				{
					UE_LOG(LogTemp, Log, TEXT("Start Countdown"));
					int32 UniqueID = EGPC->PlayerIndex;
					if (AEGGameStateBase* EGGS = Cast<AEGGameStateBase>(GetWorld()->GetGameState()))
					{
						UE_LOG(LogTemp, Log, TEXT("EGGS is not NULL"));
						EGGS->CheckRoomLeader(UniqueID);
					}
				}
			}

		}
	}	
}