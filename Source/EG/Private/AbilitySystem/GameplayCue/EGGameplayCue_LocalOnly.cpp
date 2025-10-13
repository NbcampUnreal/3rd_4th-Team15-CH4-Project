// EGGameplayCue_LocalOnly.cpp (JJM)


#include "AbilitySystem/GameplayCue/EGGameplayCue_LocalOnly.h"
#include "AbilitySystemGlobals.h"
#include "EGLog.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UEGGameplayCue_LocalOnly::HandleGameplayCue(AActor* Target, EGameplayCueEvent::Type EventType,
                                                 const FGameplayCueParameters& Parameters)
{
	if (!Target || !SFX)
	{
		EG_LOG(LogJM, Warning, TEXT("No SFX or Target"));
		return;
	}

	if (APawn* Pawn = Cast<APawn>(Target))	// Local Client 만 SFX 재생
	{
		if (!Pawn->IsLocallyControlled())
		{
			EG_LOG(LogJM, Warning, TEXT("Target is not LocalPawn"));
			return;
		}
	}
	
	switch (EventType)
	{
	case EGameplayCueEvent::OnActive:
		EG_LOG(LogJM, Log, TEXT("On Active"));
		PlaySFXLoopLocalOnly(Target);
		break;
		
	case EGameplayCueEvent::WhileActive:
		EG_LOG(LogJM, Log, TEXT("While Active"));
		break;

	case EGameplayCueEvent::Removed:
		EG_LOG(LogJM, Log, TEXT("Removed"));
		StopSFXLocalOnly();
		break;

	default:
		break;
	}
}

void UEGGameplayCue_LocalOnly::PlaySFXLoopLocalOnly(AActor* MyTarget)
{
	StopSFXLocalOnly();	// 새 아이템 먹으면 사운드 새로 재생
	
	// JM : 해당 SFX도 루핑 설정 해줘야 반복재생이 됨
	AudioComponent = UGameplayStatics::SpawnSound2D(
		GetWorld(),
		SFX,
		VolumeMultiplier,
		1.0f,
		0.0f,
		nullptr,
		true
	);
}

void UEGGameplayCue_LocalOnly::StopSFXLocalOnly()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent = nullptr;
	}
	else
	{
		EG_LOG(LogJM, Warning, TEXT("AudioComponent is null"));
	}
}
