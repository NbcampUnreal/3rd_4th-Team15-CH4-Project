// BTTask_RandomWait.h

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomWait.generated.h"

/**
 * 
 */
UCLASS()
class EG_API UBTTask_RandomWait : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RandomWait();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float RemainingTime;
};
