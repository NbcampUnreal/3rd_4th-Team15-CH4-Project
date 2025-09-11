// AIConfigData.h

#pragma once

#include "AI/EGAIState.h"
#include "Engine/DataAsset.h"
#include "AIConfigData.generated.h"

USTRUCT(BlueprintType)
struct FAIActionProbability
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAIState ActionState = EAIState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Probability = 0.f;
};

UCLASS(BlueprintType)
class EG_API UAIConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UAIConfigData();

	// Idle Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Idle")
	float MinWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Idle")
	float MaxWaitTime;

	// Move Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move")
	float MoveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move")
	float MoveSpeed;

	// Action State Probability
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAIActionProbability> ActionProbabilities;
};
