// BGMManagerSubsystem.cpp (작성자 : JJM)

#include "Sounds/BGMManagerSubsystem.h"
#include "EGLog.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sounds/BGMDataAsset.h"


void UBGMManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	
	Super::Initialize(Collection);
	
	if (IsRunningDedicatedServer())	// 0. 서버에서는 실행하지 않음
	{
		EG_LOG(LogJM, Warning, TEXT("Server BGM is not inited"));
		return;
	}
	
	// 1. 에디터에 설정해 둔 데이터 에셋을 동기로드 (BP로 만들 수 없어서, DA를 에디터에서 할당할 수가 없음)
	TSoftObjectPtr<UBGMDataAsset> DataAssetRef{ FSoftObjectPath(TEXT("/Game/OhMyEgg/Sounds/DA_BGMList.DA_BGMList")) };
	if (UBGMDataAsset* Loaded = DataAssetRef.LoadSynchronous())
	{
        BGMDataAsset = Loaded;
    }
    else
    {
        EG_LOG(LogJM, Error, TEXT("Failed to load BGM DataAsset: %s"), *DataAssetRef.ToString());
    }

	// 3. 맵(레벨) 로드가 완료될 때마다 OnPostLoadMap 함수를 실행하도록 '델리게이트'에 바인딩
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UBGMManagerSubsystem::OnPostLoadMap);
	
	EG_LOG(LogJM, Log, TEXT("End"));
}

void UBGMManagerSubsystem::Deinitialize()
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	
	// 델리게이트 바인딩을 해제하여 메모리 누수를 방지
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Deinitialize();
	
	EG_LOG(LogJM, Log, TEXT("End"));
}

void UBGMManagerSubsystem::PlayBGM(EBGMType InLevelName)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	
    USoundBase* Sound = ResolveBGMSound(InLevelName, BGMDataAsset);
    if (!Sound)
    {
        EG_LOG(LogJM, Warning, TEXT("BGM sound not found or failed to load for level %s"), *StaticEnum<EBGMType>()->GetValueAsString(InLevelName));
        return;
    }

	if (InLevelName == CurrentPlayingBGM)	// 이미 같은 BGM 재생중이면 끊이지 않게 하기
	{
		EG_LOG(LogJM, Warning, TEXT("Already Playing Same BGM(%s)"), *StaticEnum<EBGMType>()->GetValueAsString(InLevelName));
		return;
	}

    // 컴포넌트가 없으면 새로 생성
    if (!BGMComponent)
    {
        BGMComponent = UGameplayStatics::CreateSound2D(
            GetGameInstance(),
            Sound,
            1.f,   // Volume
            1.f,   // Pitch
            0.f,   // StartTime
            nullptr,
            true,  // bPersistAcrossLevelTransition
            false  // bAutoDestroy (지속 재사용)
        );
        if (!BGMComponent)
        {
            EG_LOG(LogJM, Warning, TEXT("CreateSound2D failed"));
            return;
        }
        BGMComponent->bIsUISound = true; // 2D BGM 용도
    }
    else  // 컴포넌트가 있으면 재사용
    {
        BGMComponent->Stop();			// Fade Out이 의미가 없어서 Stop (바로 다른 노래 재생할거니까)
        BGMComponent->SetSound(Sound);
    }

	BGMComponent->SetVolumeMultiplier(BGMDataAsset->VolumeMultiplier);	// BGM 볼륨 조절
	BGMComponent->FadeIn(BGMDataAsset->FadeTime);						// BGM 페이드 인 
    // GMComponent->Play();							// FadeIn 적용 안하려면 바로 Play 해도 됨
    CurrentPlayingBGM = InLevelName;
	
	EG_LOG(LogJM, Log, TEXT("End"));
}

void UBGMManagerSubsystem::StopBGM()
{
	if (BGMComponent)
	{
		BGMComponent->FadeOut(BGMDataAsset->FadeTime, 0.0f);
	}
	else
	{
		EG_LOG(LogJM, Warning, TEXT("BGMComponent is null"));
	}
}

void UBGMManagerSubsystem::OnPostLoadMap(UWorld* LoadedWorld)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	
	if (!LoadedWorld || !BGMDataAsset)
	{
		EG_LOG(LogJM, Warning, TEXT(" !LoadedWorld || !BGADataAsset"));
		return;
	}

	// 현재 로드된 맵의 이름을 가져옴 (ex: "MainMenu", "Stage1")
	FString CurrentMapName = UGameplayStatics::GetCurrentLevelName(LoadedWorld, true);
	EG_LOG(LogJM, Log, TEXT("Current Map Name (Clean): %s"), *CurrentMapName); // 확인을 위해 출력

	// FString을 ELevelName Enum으로 변환합니다.
	int64 EnumValue = StaticEnum<EBGMType>()->GetValueByNameString(CurrentMapName);

	if (EnumValue != INDEX_NONE)	// Data Asset에 있는 맵인 경우
	{
		EBGMType LevelToPlay = static_cast<EBGMType>(EnumValue);
		PlayBGM(LevelToPlay);
	}
	else	// Data Asset에 없는 맵이라면 BGM 종료
	{
		PlayBGM(EBGMType::None);		
		EG_LOG(LogJM, Warning, TEXT("Map '%s' not found in BGM Data Asset. Stopping BGM."), *CurrentMapName);
	}
	EG_LOG(LogJM, Log, TEXT("End"));
}

USoundBase* UBGMManagerSubsystem::ResolveBGMSound(EBGMType InLevelName, const UBGMDataAsset* DataAsset)
{
	EG_LOG(LogJM, Log, TEXT("Start"));
	
	if (!DataAsset)
	{
		EG_LOG(LogJM, Log, TEXT("No DataAsset"));
		return nullptr;
	}

	const FBGMInfo* Info = DataAsset->BGMList.FindByPredicate(	// TArray에서 람다식 조건을 만족하는 첫 요소 반환
		[InLevelName](const FBGMInfo& It)
		{
			return It.BGMTypeName == InLevelName;
		}
	);

	if (!Info)
	{
		EG_LOG(LogJM, Log, TEXT("No %s BGM Type"), *StaticEnum<EBGMType>()->GetValueAsString(InLevelName));
		return nullptr;
	}

	EG_LOG(LogJM, Log, TEXT("End"));
	// 소프트 참조를 동기 로드
	return Info->BGMSound.LoadSynchronous();
}