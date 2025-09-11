// BTTask_CustomMoveTo.cpp

#include "AI/Tasks/BTTask_CustomMoveTo.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "AI/EGAICharacter.h"
#include "AI/EGAIController.h"
#include "AI/DataAsset/AIConfigData.h"

UBTTask_CustomMoveTo::UBTTask_CustomMoveTo()
{
	bNotifyTick = true;
	NodeName = TEXT("Custom Move To");
}

EBTNodeResult::Type UBTTask_CustomMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEGAIController* EGAIController = Cast<AEGAIController>(AIController))
		{
			if (AEGAICharacter* EGAICharacter = Cast<AEGAICharacter>(EGAIController->GetPawn()))
			{
				float Speed = EGAIController->GetConfigData()->MoveSpeed;
				EGAICharacter->GetCharacterMovement()->MaxWalkSpeed = Speed;

				if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
				{
					FVector TargetLocation = Blackboard->GetValueAsVector("TargetLocation");

					EPathFollowingRequestResult::Type Result = EGAIController->MoveToLocation(TargetLocation);

					if (Result == EPathFollowingRequestResult::Type::RequestSuccessful)
					{
						return EBTNodeResult::InProgress;
					}
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_CustomMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		EPathFollowingStatus::Type Status = AIController->GetMoveStatus();

		if (Status == EPathFollowingStatus::Idle)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
