// EGGameModeBase_Cutscene.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EGGameModeBase_Cutscene.generated.h"

/**
 * 
 */
UCLASS()
class EG_API AEGGameModeBase_Cutscene : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEGGameModeBase_Cutscene();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UFUNCTION()
	void OnCutsceneFinished();

	UFUNCTION()
	void OnPlayCutscene();

	FTimerHandle TimerHandle_Cutscene;
};
