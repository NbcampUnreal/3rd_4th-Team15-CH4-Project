//EggActor.cpp

#include "Character/Egg/EggActor.h"
#include "Net/UnrealNetwork.h"

AEggActor::AEggActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootScene);

	StaticMesh->SetIsReplicated(true);
}

int32 AEggActor::GetHealth() const
{
	return Health;
}

void AEggActor::SetHealth(int32 NewHealth)
{
	Health = NewHealth;
}

void AEggActor::CheckHealthAndDestroy()
{
	if (Health <= 0)
	{
		Destroy();
	}
}

void AEggActor::MulticastUpdateGroundState_Implementation(bool bNewIsOnGround)
{
	bIsOnGround = bNewIsOnGround;
	SetActorTickEnabled(!bIsOnGround);
}

void AEggActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority())
	{
		bool bWasOnGround = bIsOnGround;
		bIsOnGround = CheckGroundContact();
		
		UE_LOG(LogTemp, Log, TEXT("Is On Ground : %s"), bIsOnGround ? TEXT("True") : TEXT("False"));
		
		if (!bIsOnGround)
		{
			ApplyGravity(DeltaTime);
		}

		if (bWasOnGround != bIsOnGround)
		{
			MulticastUpdateGroundState(bIsOnGround);
		}
	}
}

void AEggActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
}

void AEggActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEggActor, bIsOnGround);
}

void AEggActor::ApplyGravity(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= GravityScale * DeltaTime;

	if (HasAuthority())
	{
		SetActorLocation(NewLocation);
	}
}

bool AEggActor::CheckGroundContact()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.0f, 0.0f, 20.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams);


	// 강제로 마지막에 0으로 맞춰주는 코드 필요(살짝의 위치 오차가 조금씩 있다.)
	
	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 3.f, 0, 1.f);

	return bHit;
}
