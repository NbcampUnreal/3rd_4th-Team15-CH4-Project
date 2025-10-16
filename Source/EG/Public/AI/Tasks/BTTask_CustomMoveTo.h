// BTTask_CustomMoveTo.h

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CustomMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UBTTask_CustomMoveTo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CustomMoveTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
