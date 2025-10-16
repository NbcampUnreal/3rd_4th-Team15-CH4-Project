// BTTask_SetActionState.h

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetActionState.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UBTTask_SetActionState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetActionState();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
