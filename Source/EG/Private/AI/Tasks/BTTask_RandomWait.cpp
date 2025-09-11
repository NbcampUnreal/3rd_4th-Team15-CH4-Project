// BTTask_RandomWait.cpp

#include "AI/Tasks/BTTask_RandomWait.h"

#include "AI/EGAICharacter.h"
#include "AI/EGAIController.h"
#include "AI/DataAsset/AIConfigData.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RandomWait::UBTTask_RandomWait()
	: RemainingTime(0.f)
{
	NodeName = TEXT("Random Wait");
}

EBTNodeResult::Type UBTTask_RandomWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEGAIController* EGAIController = Cast<AEGAIController>(AIController))
		{
			if (AEGAICharacter* EGAICharacter = Cast<AEGAICharacter>(EGAIController->GetPawn()))
			{
				if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
				{
					float MinWaitTime = EGAIController->GetConfigData()->MinWaitTime;
					float MaxWaitTime = EGAIController->GetConfigData()->MaxWaitTime;

					RemainingTime = EGAICharacter->RandomStream.FRandRange(MinWaitTime, MaxWaitTime);
					RemainingTime += (EGAICharacter->GetUniqueID() % 100) * 0.01f;

					Blackboard->SetValueAsFloat("WaitTime", RemainingTime);

					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
