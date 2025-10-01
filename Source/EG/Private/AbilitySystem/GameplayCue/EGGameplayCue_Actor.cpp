// EGGameplayCue_Actor.cpp (JJM)


#include "AbilitySystem/GameplayCue/EGGameplayCue_Actor.h"

#include "EGLog.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


void AEGGameplayCue_Actor::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType,
												 const FGameplayCueParameters& Parameters)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	if (!MyTarget || !SFX)
		return;

	switch (EventType)
	{
	case EGameplayCueEvent::OnActive:
	case EGameplayCueEvent::WhileActive:
		{
			PlaySFXLoop(MyTarget);
			break;
		}

	case EGameplayCueEvent::Removed:
		{
			StopSFX();
			break;
		}

	default:
		break;
	}
	EG_LOG(LogJM, Log, TEXT("End"));
}

/*
void AEGGameplayCue_Actor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
		AudioComponent = nullptr;
	}
	Super::EndPlay(EndPlayReason);
	EG_LOG(LogJM, Log, TEXT("End"));
}*/

void AEGGameplayCue_Actor::PlaySFXLoop(AActor* MyTarget)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	/*if (!AudioComponent)
	{
		EG_LOG(LogJM, Warning, TEXT("AudioComponent is null"));
		return;
	}*/

	if (AudioComponent && AudioComponent->IsPlaying())
	{
		EG_LOG(LogJM, Warning, TEXT("AudioComponent is already playing"));
		return;
	}
	
	// AudioComponent를 생성하고 Target에 Attach
	AudioComponent = UGameplayStatics::SpawnSoundAttached(
		SFX,
		MyTarget->GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		EAttachLocation::KeepRelativeOffset,
		true
	);
	EG_LOG(LogJM, Log, TEXT("end"));
}

void AEGGameplayCue_Actor::StopSFX()
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}
	EG_LOG(LogJM, Log, TEXT("End"));
}

/*
bool AEGGameplayCue_Actor::OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	if (!MyTarget || !SFX)
	{
		EG_LOG(LogJM, Warning, TEXT("No Target or SFX"));
		return false;
	}
	
	AudioComponent = UGameplayStatics::SpawnSoundAttached(
			SFX,
			MyTarget->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset,
			bIsLoop);

	EG_LOG(LogJM, Log, TEXT("End"));
	return Super::OnActive_Implementation(MyTarget, Parameters);
}

bool AEGGameplayCue_Actor::OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}
	EG_LOG(LogJM, Log, TEXT("End"));
	return Super::OnRemove_Implementation(MyTarget, Parameters);
}

void AEGGameplayCue_Actor::GameplayCueFinishedCallback()
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	Super::GameplayCueFinishedCallback();
	EG_LOG(LogJM, Log, TEXT("End"));
}
*/
