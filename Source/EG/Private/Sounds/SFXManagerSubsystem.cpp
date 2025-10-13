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
	// JM : 월드 이동시 AudioComponent가 null 참조 되는 문제 해결
	// Deinitialize에서 모든 AudioComponent 정지, 등록해제
	for (TPair<ESFXType, TObjectPtr<UAudioComponent>>& Pair : ActiveSFXMap)
	{
		UAudioComponent* AudioComp = Pair.Value;
		if (AudioComp)
		{
			// 1. 재생 중인 사운드 정지
			if (AudioComp->IsPlaying())
			{
				AudioComp->Stop();
			}

			// 2. 컴포넌트를 씬과 월드에서 등록 해제
			// 이게 가장 중요합니다. 씬에 등록된 채로 GC가 월드를 파괴하면 크래시가 납니다.
			if (AudioComp->IsRegistered())
			{
				AudioComp->UnregisterComponent();
			}

			// 3. 컴포넌트 파괴 (NewObject로 생성했으므로 명시적 파괴가 필요합니다)
			// UAudioComponent::DestroyComponent()는 ActorComponent가 아닌 경우 바로 MarkPendingKill()을 호출합니다.
			AudioComp->DestroyComponent();
		}
	}
    
	// 4. 맵 초기화
	ActiveSFXMap.Empty();
	
	Super::Deinitialize();
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
	// JM : 월드 이동시 AudioComponent가 null 참조 되는 문제 해결
	// NewObject<UAudioComponent>(this) 대신 World의 WorldSettings를 오너로 지정
	UAudioComponent* NewComp = NewObject<UAudioComponent>(World->GetWorldSettings());
	NewComp->bAutoActivate = false;
    
	// UAudioComponent가 ActorComponent를 상속받았으므로, RegisterComponentWithWorld(World)를 호출해야 합니다.
	// 컴포넌트를 World에 등록
	NewComp->RegisterComponentWithWorld(World); 
    
	ActiveSFXMap.Add(InType, NewComp);
	return NewComp;
	

	/*UAudioComponent* NewComp = NewObject<UAudioComponent>(this);
	NewComp->bAutoActivate = false;
	NewComp->RegisterComponentWithWorld(World);	// World 에 등록
	ActiveSFXMap.Add(InType, NewComp);
	return NewComp;*/

	/*UAudioComponent* NewComp = NewObject<UAudioComponent>(World->GetWorldSettings());
	NewComp->bAutoActivate = false;
	NewComp->RegisterComponent();
	ActiveSFXMap.Add(InType, NewComp);
	return NewComp;*/
}
