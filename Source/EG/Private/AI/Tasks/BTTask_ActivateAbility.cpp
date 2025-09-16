// BTTask_ActivateAbility.cpp


#include "AI/Tasks/BTTask_ActivateAbility.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "EGLog.h"
#include "AI/EGAICharacter.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	NodeName = TEXT("Activate Ability");
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		if (AEGAICharacter* AICharacter = Cast<AEGAICharacter>(AIController->GetPawn()))
		{
			if (UAbilitySystemComponent* ASC = AICharacter->GetAbilitySystemComponent())
			{
				if (AbilityToActivate)
				{
					bool bActivated = ASC->TryActivateAbilityByClass(AbilityToActivate);
					if (bActivated)
					{
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
