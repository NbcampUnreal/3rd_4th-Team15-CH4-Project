// EGAICharacter.cpp

#include "AI/EGAICharacter.h"

#include "AbilitySystemComponent.h"
#include "EGLog.h"
#include "AI/EGAIController.h"
#include "AI/EGAIState.h"
#include "AI/AbilitySystem/AttributeSet/EGAI_AttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"

AEGAICharacter::AEGAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UEGAI_AttributeSet>(TEXT("AttributeSet"));
}

void AEGAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// Initialize random stream
		int32 Seed = GetUniqueID() ^ FDateTime::Now().GetMillisecond();
		RandomStream.Initialize(Seed);

		GiveAbilities();
	}
}

UAbilitySystemComponent* AEGAICharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEGAICharacter::GiveAbilities()
{
	if (!AbilitySystemComponent)
	{
		EG_LOG(LogKH, Warning, TEXT("AI Character : No AbilitySystemComponent"));
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : GrantedAbilityClasses)
	{
		if (AbilityClass)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
		}
	}
}

void AEGAICharacter::OnAngryMode()
{
	if (HasAuthority())
	{
		if (AEGAIController* EGAIController = Cast<AEGAIController>(GetController()))
		{
			if (UBlackboardComponent* Blackboard = EGAIController->GetBlackboardComponent())
			{
				EG_LOG(LogKH, Log, TEXT("%s : Angry"), *this->GetName());
				
				Blackboard->SetValueAsEnum("ActionState", static_cast<uint8>(EAIState::Angry));
			}
		}
	}
}
