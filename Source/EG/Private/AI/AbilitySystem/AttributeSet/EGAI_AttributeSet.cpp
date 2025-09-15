// EGAI_AttributeSet.cpp

#include "AI/AbilitySystem/AttributeSet/EGAI_AttributeSet.h"

#include "Net/UnrealNetwork.h"

UEGAI_AttributeSet::UEGAI_AttributeSet()
{
	InitMoveSpeed(100.f);
}

void UEGAI_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEGAI_AttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UEGAI_AttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEGAI_AttributeSet, MoveSpeed, OldMoveSpeed);
}
