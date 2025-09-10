// BTTask_SetActionState.cpp


#include "AI/Tasks/BTTask_SetActionState.h"

#include "AI/EGAIController.h"
#include "AI/DataAsset/AIConfigData.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetActionState::UBTTask_SetActionState()
{
	NodeName = TEXT("Set Action State");
}

EBTNodeResult::Type UBTTask_SetActionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEGAIController* EGAIController = Cast<AEGAIController>(AIController))
		{
			if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
			{
				float TotalWeight = 0.f;
				for (auto& Entry : EGAIController->GetConfigData()->ActionProbabilities)
				{
					TotalWeight += Entry.Probability;
				}

				float RandomValue = FMath::FRandRange(0.f, TotalWeight);
				float AccumulatedWeight = 0.f;
				for (auto& Entry : EGAIController->GetConfigData()->ActionProbabilities)
				{
					AccumulatedWeight += Entry.Probability;
					if (RandomValue <= AccumulatedWeight)
					{
						Blackboard->SetValueAsEnum("ActionState", static_cast<uint8>(Entry.ActionState));

						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
