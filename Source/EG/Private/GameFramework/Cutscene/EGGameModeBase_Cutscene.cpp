// EGGameModeBase_Cutscene.cpp


#include "GameFramework/Cutscene/EGGameModeBase_Cutscene.h"

#include "EGLog.h"
#include "GameFramework/EGGameInstance.h"
#include "GameFramework/EGPlayerController.h"
#include "GameFramework/EGPlayerState.h"

AEGGameModeBase_Cutscene::AEGGameModeBase_Cutscene()
{
	bUseSeamlessTravel = true;
}

void AEGGameModeBase_Cutscene::BeginPlay()
{
	Super::BeginPlay();

	EG_LOG(LogKH, Log, TEXT("Cutscene Game Mode BeginPlay"));

	if (HasAuthority())
	{
		if (UEGGameInstance* GI = GetGameInstance<UEGGameInstance>())
		{
			const TArray<FFinalResult>& Results = GI->GetFinalResults();

			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				if (AEGPlayerController* PC = Cast<AEGPlayerController>(It->Get()))
				{
					int32 PlayerId = PC->GetPlayerState<AEGPlayerState>()->GetPlayerId();
					const FFinalResult* Found = Results.FindByPredicate(
						[PlayerId](const FFinalResult& Result) {return Result.PlayerId == PlayerId;}
						);

					if (Found)
					{
						PC->ClientRPC_PlayEndingSequence(Found->bIsWinner);
					}
				}
			}
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEGGameModeBase_Cutscene::OnCutsceneFinished, 12.f, false);
	}
}

void AEGGameModeBase_Cutscene::OnCutsceneFinished()
{
	GetWorld()->ServerTravel("/Game/UI/Map/LobbyMap?listen");
}
