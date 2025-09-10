// BTTask_RandomWait.cpp

#include "AI/Tasks/BTTask_RandomWait.h"

#include "AI/EGAIController.h"
#include "AI/DataAsset/AIConfigData.h"

UBTTask_RandomWait::UBTTask_RandomWait()
	: RemainingTime(0.f)
{
	bNotifyTick = true;
	NodeName = TEXT("Random Wait");
}

EBTNodeResult::Type UBTTask_RandomWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEGAIController* EGAIController = Cast<AEGAIController>(AIController))
		{
			float MinWaitTime = EGAIController->GetConfigData()->MinWaitTime;
			float MaxWaitTime = EGAIController->GetConfigData()->MaxWaitTime;

			RemainingTime = FMath::FRandRange(MinWaitTime, MaxWaitTime);

			return EBTNodeResult::InProgress;
		}
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_RandomWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	RemainingTime -= DeltaSeconds;
	if (RemainingTime <= 0.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
