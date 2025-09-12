// EGAICharacter.cpp

#include "AI/EGAICharacter.h"

#include "AbilitySystemComponent.h"
#include "EGLog.h"
#include "AI/AbilitySystem/AttributeSet/EGAI_AttributeSet.h"

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

	if (PeckAbilityClass)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(PeckAbilityClass, 1, INDEX_NONE, this));
	}
	if (LayEggAbilityClass)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(LayEggAbilityClass, 1, INDEX_NONE, this));
	}
}
