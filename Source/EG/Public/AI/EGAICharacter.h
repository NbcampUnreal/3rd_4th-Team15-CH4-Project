// EGAICharacter.h

#pragma once

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "EGAICharacter.generated.h"

class UGameplayAbility;
class UEGAI_AttributeSet;

UCLASS()
class EG_API AEGAICharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEGAICharacter();

protected:
	virtual void BeginPlay() override;

#pragma region GAS
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditDefaultsOnly, Category = "AICharacter|GAS")
	TSubclassOf<UGameplayAbility> PeckAbilityClass;

	UPROPERTY(EditDefaultsOnly, Category = "AICharacter|GAS")
	TSubclassOf<UGameplayAbility> LayEggAbilityClass;

private:
	UPROPERTY(VisibleAnywhere, Category = "AICharacter|GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UEGAI_AttributeSet* AttributeSet;

	void GiveAbilities();
	
#pragma endregion
	
public:
	UPROPERTY()
	FRandomStream RandomStream;
};
