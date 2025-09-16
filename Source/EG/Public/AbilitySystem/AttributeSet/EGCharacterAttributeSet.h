// EGCharacterAttributeSet.h

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EGCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class EG_API UEGCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEGCharacterAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "EggEnergy", ReplicatedUsing = OnRep_EggEnergy)
	FGameplayAttributeData EggEnergy;
	ATTRIBUTE_ACCESSORS(UEGCharacterAttributeSet, EggEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UEGCharacterAttributeSet, Stamina)

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_EggEnergy(const FGameplayAttributeData& OldEggEnergy);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldEggEnergy);
};
