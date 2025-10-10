// SFXManagerSubsystem.cpp (JJM)


#include "Sounds/SFXManagerSubsystem.h"

#include "EGLog.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USFXManagerSubsystem::USFXManagerSubsystem()
{
	// 하드 로딩(패키징 과정에서 data asset이 제외될 수 있음)
	static ConstructorHelpers::FObjectFinder<USFXDataAsset> DataAssetRef(TEXT("/Game/OhMyEgg/Sounds/DA_SFXList.DA_SFXList"));
	if (DataAssetRef.Succeeded())
	{
		SFXDataAsset = DataAssetRef.Object;
	}
	else
	{
		EG_LOG(LogJM, Error, TEXT("Failed to load BGM DataAsset: %s"), TEXT("/Game/OhMyEgg/Sounds/DA_SFXList.DA_SFXList"));
	}
}

void USFXManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (IsRunningDedicatedServer())
	{
		EG_LOG(LogJM, Warning, TEXT("Server SFX Manager is not init"));
		return;
	}

	// 1. 에디터에 설정해 둔 데이터 에셋을 동기로드 (BP로 만들 수 없어서, DA를 에디터에서 할당할 수가 없음)
	// JM : 런타임 로드가 패키징에 제외될 수 있을수도? 있어서 생성자에서 강한참조로 바꿈
	/*TSoftObjectPtr<USFXDataAsset> DataAssetRef{ FSoftObjectPath(TEXT("/Game/OhMyEgg/Sounds/DA_SFXList.DA_SFXList")) };
	if (USFXDataAsset* Loaded = DataAssetRef.LoadSynchronous())
	{
		SFXDataAsset = Loaded;
	}
	else
	{
		EG_LOG(LogJM, Error, TEXT("Failed to load SFX DataAsset: %s"), *DataAssetRef.ToString());
	}*/
}

void USFXManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	// 불필요
}

void USFXManagerSubsystem::PlaySFXLocalClientOnly(ESFXType InType, UWorld* World)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	if (!SFXDataAsset)
	{
		EG_LOG(LogJM, Warning, TEXT("No SFXDataAsset"));
		return;
	}

	USoundBase* Sound = SFXDataAsset->GetSFXSound(InType);
	if (!Sound)
	{
		EG_LOG(LogJM, Warning, TEXT("No Sound or WorldContext"));
		return;
	}

	//UWorld* World = WorldContext->GetWorld();
	if (!World)
	{
		EG_LOG(LogJM, Warning, TEXT("No World"));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC || !PC->IsLocalController())
	{
		EG_LOG(LogJM, Warning, TEXT("No Local PlayerController"));
		return;
	}

	UAudioComponent* AudioComp = GetAudioComponent(World, InType);
	if (AudioComp)
	{
		AudioComp->SetSound(Sound);
		AudioComp->Play();
	}
	else
	{
		EG_LOG(LogJM, Warning, TEXT("Failed to get AudioComponent"));
	}
	EG_LOG(LogJM, Log, TEXT("End"));
}

/*void USFXManagerSubsystem::PlaySFXAtLocation(ESFXType InType, UObject* WorldContext, const FVector& Location)
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
}*/

void USFXManagerSubsystem::StopSFX(ESFXType InType)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	if (UAudioComponent* AudioComp = ActiveSFXMap.FindRef(InType))
	{
		if (AudioComp->IsPlaying())
		{
			AudioComp->Stop();
		}
		else
		{
			EG_LOG(LogJM, Warning, TEXT("AudioComponent is not playing"));
		}
	}
	else
	{
		EG_LOG(LogJM, Warning, TEXT("No AudioComponent for %s"), *StaticEnum<ESFXType>()->GetValueAsString(InType));
	}
	EG_LOG(LogJM, Log, TEXT("End"));
}

UAudioComponent* USFXManagerSubsystem::GetAudioComponent(UWorld* World, ESFXType InType)
{
	if (!IsValid(World))
	{
		EG_LOG(LogJM, Warning, TEXT("Invalid World"));
		return nullptr;
	}

	// 이미 존재하는 AudioComponent가 있으면 재사용
	if (ActiveSFXMap.Contains(InType))
	{
		UAudioComponent* ExistingComp = ActiveSFXMap[InType];
		if (ExistingComp)
		{
			if (ExistingComp->IsPlaying())
			{
				ExistingComp->Stop();	// 이미 같은 타입이 재생중이면 Stop 후 재사용
			}
			return ExistingComp;
		}
	}

	// 기존 AudioComponent가 없다면 새로 생성
	UAudioComponent* NewComp = NewObject<UAudioComponent>(this);
	NewComp->bAutoActivate = false;
	NewComp->RegisterComponentWithWorld(World);	// World 에 등록
	ActiveSFXMap.Add(InType, NewComp);
	return NewComp;
	/*UAudioComponent* NewComp = NewObject<UAudioComponent>(World->GetWorldSettings());
	NewComp->bAutoActivate = false;
	NewComp->RegisterComponent();
	ActiveSFXMap.Add(InType, NewComp);
	return NewComp;*/
}
