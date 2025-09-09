// BTTask_FindRandomLocation.cpp

#include "AI/Tasks/BTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
	: SearchRadius(500.0f)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (APawn* Pawn = AIController->GetPawn())
		{
			if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld()))
			{
				FVector Origin = Pawn->GetActorLocation();
				FNavLocation RandomPoint;

				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, RandomPoint))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", RandomPoint.Location);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
