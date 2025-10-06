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

void AEGGameplayCue_Actor::PlaySFXLoop(AActor* MyTarget)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	
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
		true,
		1.0f,
		1.0f,
		0.0f,
		SFXAttenuation
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
