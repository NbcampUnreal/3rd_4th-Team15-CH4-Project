// EGAIController.h

#pragma once

#include "Runtime/AIModule/Classes/AIController.h"
#include "EGAIController.generated.h"

class UAIConfigData;

UCLASS()
class EG_API AEGAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEGAIController();

	UAIConfigData* GetConfigData() const;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "AIController|AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AIController|Config")
	UAIConfigData* ConfigData;
};
