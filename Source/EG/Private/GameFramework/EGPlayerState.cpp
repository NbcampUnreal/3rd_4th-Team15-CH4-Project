#include "GameFramework/EGPlayerState.h"
#include "GameFramework/EGGameStateBase.h"
#include "Net/UnrealNetwork.h"

AEGPlayerState::AEGPlayerState()
{
	bReplicates = true;
}

void AEGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEGPlayerState, PlayerEggCount);
}

void AEGPlayerState::OnRep_PlayerEggCount()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_PlayerEggCount: %s now has %d eggs"), *GetName(), PlayerEggCount);
}

void AEGPlayerState::ServerAddEgg_Implementation(int32 Amount)
{
	AddEgg_Internal(Amount);
}

void AEGPlayerState::ServerRemoveEgg_Implementation(int32 Amount)
{
	RemoveEgg_Internal(Amount);
}

void AEGPlayerState::AddEgg_Internal(int32 Amount)
{
	PlayerEggCount = FMath::Max(0, PlayerEggCount + Amount);

	UE_LOG(LogTemp, Log, TEXT("%s gained eggs: %d (Total: %d)"), *GetName(), Amount, PlayerEggCount);

	if (AEGGameStateBase* GS = GetWorld()->GetGameState<AEGGameStateBase>())
	{
		GS->UpdateLeaderboard();
	}
}

void AEGPlayerState::RemoveEgg_Internal(int32 Amount)
{
	PlayerEggCount = FMath::Max(0, PlayerEggCount - Amount);

	UE_LOG(LogTemp, Log, TEXT("%s lost eggs: %d (Total: %d)"), *GetName(), Amount, PlayerEggCount);

	if (AEGGameStateBase* GS = GetWorld()->GetGameState<AEGGameStateBase>())
	{
		GS->UpdateLeaderboard();
	}
}

void AEGPlayerState::ResetEggCount()
{
	PlayerEggCount = 0;
	OnRep_PlayerEggCount();
}