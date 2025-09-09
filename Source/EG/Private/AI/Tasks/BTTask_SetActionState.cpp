// BTTask_SetActionState.cpp


#include "AI/Tasks/BTTask_SetActionState.h"

#include "AI/EGAIState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetActionState::UBTTask_SetActionState()
{
	NodeName = TEXT("Set Action State");
}

EBTNodeResult::Type UBTTask_SetActionState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		int32 RandomValue = FMath::RandRange(0, static_cast<int32>(EAIState::Count) - 1);
		EAIState NewState = static_cast<EAIState>(RandomValue);

		Blackboard->SetValueAsEnum("ActionState", static_cast<uint8>(NewState));

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
