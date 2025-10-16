#include "GameFrameWork/EGStartingButton.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/EGGameStateBase.h"


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
	if (OtherActor)
	{
		AEGChickenCharacter* Character = Cast<AEGChickenCharacter>(OtherActor);
		if (Character)
		{
			if (ACharacter* CharacterPtr = Cast<ACharacter>(Character))
			{
				if (AEGPlayerController* EGPC = Cast<AEGPlayerController>(CharacterPtr->GetController()))
				{
					int32 UniqueID = EGPC->PlayerIndex;
					if (AEGGameStateBase* EGGS = Cast<AEGGameStateBase>(GetWorld()->GetGameState()))
					{
						EGGS->CheckRoomLeader();
					}
				}
			}

		}
	}	
}