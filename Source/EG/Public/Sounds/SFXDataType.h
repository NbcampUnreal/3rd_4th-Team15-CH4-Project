// SFXDataType.h (JJM)

#pragma once

#include "CoreMinimal.h"
#include "SFXDataType.generated.h"

UENUM(BlueprintType)
enum class ESFXType : uint8
{
	None		UMETA(DisplayName = "None"),
	BlockSkill	UMETA(DisplayName = "BlockSkill"),
	ItemUse		UMETA(DisplayName = "ItemUse"),
	GameStart	UMETA(DisplayName = "GameStart"),
	GameOver	UMETA(DisplayName = "GameOver")
};

USTRUCT(BlueprintType)
struct FSFXInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESFXType SFXType = ESFXType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> SFXSound;
};