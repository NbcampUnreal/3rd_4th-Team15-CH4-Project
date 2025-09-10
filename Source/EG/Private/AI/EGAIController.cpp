// EGAIController.cpp

#include "AI/EGAIController.h"

AEGAIController::AEGAIController()
	: BehaviorTreeAsset(nullptr)
	, ConfigData(nullptr)
{
	bAttachToPawn = true;
}

UAIConfigData* AEGAIController::GetConfigData() const
{
	return ConfigData;
}

void AEGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

