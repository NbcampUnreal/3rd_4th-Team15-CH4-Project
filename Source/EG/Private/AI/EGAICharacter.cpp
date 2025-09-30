// EGAICharacter.cpp

#include "AI/EGAICharacter.h"

#include "AbilitySystemComponent.h"
#include "EGLog.h"
#include "AI/EGAIController.h"
#include "AI/EGAIState.h"
#include "AI/AbilitySystem/AttributeSet/EGAI_AttributeSet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

void AEGAICharacter::OnAngryMode(FVector AttackLocation)
{
	if (HasAuthority())
	{
		if (AEGAIController* EGAIController = Cast<AEGAIController>(GetController()))
		{
			if (UBlackboardComponent* Blackboard = EGAIController->GetBlackboardComponent())
			{
				EG_LOG(LogKH, Log, TEXT("%s : Angry"), *this->GetName());
				
				Blackboard->SetValueAsEnum("ActionState", static_cast<uint8>(EAIState::Angry));

				FVector curLocation = GetActorLocation();
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(curLocation, AttackLocation);

				LookAtRotation.Pitch = 0.f;
				LookAtRotation.Roll = 0.f;

				SetActorRotation(LookAtRotation);
			}
		}
	}
}
