// EGGameplayCue_Status_Peck.cpp (JJM)

#include "AbilitySystem/GameplayCue/EGGameplayCue.h"

#include "EGLog.h"
#include "Kismet/GameplayStatics.h"

void UEGGameplayCue::HandleGameplayCue(AActor* Target, EGameplayCueEvent::Type EventType,
                                                   const FGameplayCueParameters& Parameters)
{
	if (EventType == EGameplayCueEvent::Executed && SFX)
	{
		// CueParameters.Location이 세팅되어있으면 그 위치에서 재생
		FVector Location;
		if (Parameters.Location.IsNearlyZero())
		{
			Location = Target->GetActorLocation();
			EG_LOG(LogJM, Warning, TEXT("Use Target Actor Location"));
		}
		else
		{
			Location = FVector(Parameters.Location);	// NetQuntize10 -> FVector 변환
			EG_LOG(LogJM, Warning, TEXT("Use Parameters Location"));
		}
		UGameplayStatics::PlaySoundAtLocation(Target, SFX, Location, 1.0f, 1.0f, 0.0f, SFXAttenuation);
		EG_LOG(LogJM, Warning, TEXT("Play SFX"));
	}
	else
	{
		EG_LOG(LogJM, Error, TEXT("No SFX or CueEvent type %s"), *StaticEnum<EGameplayCueEvent::Type>()->GetValueAsString(EventType));
	}
}
