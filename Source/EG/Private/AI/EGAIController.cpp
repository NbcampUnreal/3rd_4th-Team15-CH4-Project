// EGAIController.cpp

#include "AI/EGAIController.h"

AEGAIController::AEGAIController()
	: BehaviorTreeAsset(nullptr)
{
	bAttachToPawn = true;
}

void AEGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

