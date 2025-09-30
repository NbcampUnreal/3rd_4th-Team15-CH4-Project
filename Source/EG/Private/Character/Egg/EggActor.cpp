//EggActor.cpp

#include "Character/Egg/EggActor.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/EGPlayerState.h"
#include "Net/UnrealNetwork.h"

AEggActor::AEggActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->SetIsReplicated(true);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(true);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_PhysicsBody);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	StaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	StaticMesh->SetMassOverrideInKg(NAME_None, 20.0f, true); // 알 무게 20kg
	StaticMesh->SetLinearDamping(1.5f); // 공기 저항 크게
	StaticMesh->SetAngularDamping(1.5f); // 회전 저항 크게

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AEggActor::OnPawnOverlap);
}

void AEggActor::ApplyDamageAndCheckDestroy(int32 Damage, AActor* DamagedActor)
{
	Health -= Damage;

	if (Health <= 0)
	{
		if (!IsValid(AbilityClass))
		{
			if (HasAuthority())
			{
				if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
				{
					if (AController* OwnerController = OwnerPawn->GetController())
					{
						if (AEGPlayerState* EGPS = Cast<AEGPlayerState>(OwnerController->PlayerState))
						{
							EGPS->RemoveEgg_Internal(1); 
						}
					}
				}
			}
			Destroy();
		}
	}
}

UAbilitySystemComponent* AEggActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEggActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);

	if (HasAuthority())
	{
		if (IsValid(AbilitySystemComponent))
		{
			AbilitySystemComponent->InitAbilityActorInfo(this, this);

			if (IsValid(AbilityClass))
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0, this));
			}
		}
	}
}

void AEggActor::OnPawnOverlap(UPrimitiveComponent* OverlappedComp,
                              AActor* OtherActor,
                              UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex,
                              bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("AI") || OtherActor->IsA(APawn::StaticClass()))
		{
			FVector PushDir = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
			StaticMesh->AddImpulse(PushDir * 600.0f, NAME_None, true);
		}
	}
}