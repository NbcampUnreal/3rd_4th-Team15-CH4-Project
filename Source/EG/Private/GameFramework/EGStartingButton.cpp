#include "GameFrameWork/EGStartingButton.h"
#include "EGLog.h"
#include "Character/EGChickenCharacter.h"
#include "GameFramework/EGGameModeBase.h"
#include "Kismet/GameplayStatics.h"

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

			if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()))
			{
				if (AEGGameModeBase* EGGameMode = Cast<AEGGameModeBase>(GameMode))
				{
					UE_LOG(LogMS, Log, TEXT("Game shoot"));
					EGGameMode->GameStart();
				}
			}
		}
	}	
}