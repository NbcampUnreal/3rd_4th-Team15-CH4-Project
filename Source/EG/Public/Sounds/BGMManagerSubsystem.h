// BGMManagerSubsystem.h (작성자 : JJM)

#pragma once

#include "CoreMinimal.h"
#include "BGMDataTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BGMManagerSubsystem.generated.h"

class UBGMDataAsset;
/**
 * 
 */
UCLASS()
class EG_API UBGMManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void PlayBGM(EBGMType InLevelName);					// 레벨 이름을 받아 BGM 재생
	void StopBGM();										// BGM 정지
	
private:
	void OnPostLoadMap(UWorld* LoadedWorld);			// 레벨이 로드 될 때마다 해당 함수 호출(Bind)
	USoundBase* ResolveBGMSound(EBGMType InLevelName, const UBGMDataAsset* DataAsset);	// 재생할 SoundBase 찾기

private:
	UPROPERTY()
	TObjectPtr<UBGMDataAsset> BGMDataAsset;				// BGM 정보가 담길 데이터 에셋 정보

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMComponent;			// 현재 재생중인 BGM을 제어하기 위한 오디오 컴포넌트

	EBGMType CurrentPlayingBGM = EBGMType::None;
	
};
