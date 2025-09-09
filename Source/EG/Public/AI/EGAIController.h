// EGAIController.h

#pragma once

#include "Runtime/AIModule/Classes/AIController.h"
#include "EGAIController.generated.h"

UCLASS()
class EG_API AEGAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEGAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "AIController|AI")
	UBehaviorTree* BehaviorTreeAsset;
};
