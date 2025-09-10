// BTTask_FindRandomLocation.cpp

#include "AI/Tasks/BTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AI/EGAIController.h"
#include "AI/DataAsset/AIConfigData.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEGAIController* EGAIController = Cast<AEGAIController>(AIController))
		{
			if (APawn* Pawn = EGAIController->GetPawn())
			{
				if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld()))
				{
					FVector Origin = Pawn->GetActorLocation();
					FNavLocation RandomPoint;

					if (NavSys->GetRandomPointInNavigableRadius(Origin, EGAIController->GetConfigData()->MoveRadius, RandomPoint))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", RandomPoint.Location);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	
	
	return EBTNodeResult::Failed;
}
