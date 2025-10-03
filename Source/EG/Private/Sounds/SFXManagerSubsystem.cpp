// SFXManagerSubsystem.cpp (JJM)


#include "Sounds/SFXManagerSubsystem.h"

#include "EGLog.h"
#include "Kismet/GameplayStatics.h"

void USFXManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (IsRunningDedicatedServer())
	{
		EG_LOG(LogJM, Warning, TEXT("Server SFX Manager is not init"));
		return;
	}

	// 1. 에디터에 설정해 둔 데이터 에셋을 동기로드 (BP로 만들 수 없어서, DA를 에디터에서 할당할 수가 없음)
	TSoftObjectPtr<USFXDataAsset> DataAssetRef{ FSoftObjectPath(TEXT("/Game/OhMyEgg/Sounds/DA_SFXList.DA_SFXList")) };
	if (USFXDataAsset* Loaded = DataAssetRef.LoadSynchronous())
	{
		SFXDataAsset = Loaded;
	}
	else
	{
		EG_LOG(LogJM, Error, TEXT("Failed to load SFX DataAsset: %s"), *DataAssetRef.ToString());
	}
	
	EG_LOG(LogJM, Log, TEXT("End"));
}

void USFXManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	// 불필요
}

void USFXManagerSubsystem::PlaySFXLocalClientOnly(ESFXType InType, UObject* WorldContext)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	if (!SFXDataAsset)
	{
		EG_LOG(LogJM, Warning, TEXT("No SFXDataAsset"));
		return;
	}

	USoundBase* Sound = SFXDataAsset->GetSFXSound(InType);
	if (!Sound || !WorldContext)
	{
		EG_LOG(LogJM, Warning, TEXT("No Sound or WorldContext"));
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		EG_LOG(LogJM, Warning, TEXT("No World"));
		return;
	}
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (PC->IsLocalController())
		{
			UGameplayStatics::PlaySound2D(World, Sound);
		}
	}
		
	EG_LOG(LogJM, Log, TEXT("End"));
}

void USFXManagerSubsystem::PlaySFXAtLocation(ESFXType InType, UObject* WorldContext, const FVector& Location)
{
	if (!SFXDataAsset)
	{
		EG_LOG(LogJM, Warning, TEXT("No SFXDataAsset"));
		return;
	}

	USoundBase* Sound = SFXDataAsset->GetSFXSound(InType);
	if (!Sound || !WorldContext)
	{
		EG_LOG(LogJM, Warning, TEXT("No Sound or WorldContext"));
	}

	UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		EG_LOG(LogJM, Warning, TEXT("No World"));
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(World, Sound, Location);
}
