// BGMDataTypes.h (작성자 : JJM)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "BGMDataTypes.generated.h"

/*  레벨 이름을 관리할 Enum을 정의 (레벨 이름으로 BGM 재생할거라 이름 맞춰줘야함)
 *  (새로운 레벨이 추가되면 여기에 이름만 추가하면 됨)
 */
UENUM(BlueprintType)
enum class EBGMType : uint8
{
	None		UMETA(DisplayName = "None"),
	MainMenu	UMETA(DisplayName = "MainMenu"),
	Lobby		UMETA(DisplayName = "Lobby"),
	Stage1		UMETA(DisplayName = "Stage1"),
	Stage2		UMETA(DisplayName = "Stage2"),
	VoiceTestMap UMETA(DisplayName = "VoiceTestMap"),	// 테스트용
	VoiceTestMap2 UMETA(DisplayName = "VoiceTestMap2"),	// 테스트용
	Level_1		UMETA(DisplayName = "Level_1"),			// 테스트용
	Level_2		UMETA(DisplayName = "Level_2"),			// 테스트용
	Level_3		UMETA(DisplayName = "Level_3"),			// 테스트용
};

// Enum과 사운드 에셋을 묶어줄 구조체를 정의
USTRUCT(BlueprintType)
struct FBGMInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
	EBGMType BGMTypeName = EBGMType::None;		// BGM 타입을 레벨 이름으로 구분

	// USoundBase는 SoundWave, SoundCue 등 모든 사운드 타입을 받을 수 있음
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
	TObjectPtr<USoundBase> BGMSound;			// 레벨에서 재생할 사운드
};